package main

import (
	"bytes"
	"io"
	"log"
	"time"
)

func main() {
	clientReader, serverWriter := io.Pipe()
	serverReader, clientWriter := io.Pipe()

	errCh := make(chan error)

	go func() {
		errCh <- server(newUnreliableReader(serverReader), serverWriter)
	}()

	go func() {
		errCh <- client(newUnreliableReader(clientReader), clientWriter)
	}()

	for i := 0; i < 2; i++ {
		if err := <-errCh; err != nil {
			log.Fatal("error: ", err)
		}
	}
}

// Example protocol:
//
// C -> S:  ClientHello | C.cookie
// S -> C:  ServerHello | S.cookie
// C -> S:  Auth | S.cookie
// S -> C:  Auth | C.cookie
// C -> S:  Ack

const (
	tagClientHello = 0x01
	tagServerHello = 0x02
	tagAuth        = 0x03
	tagAck         = 0x04
)

// Server ---------------------------------------------------------------------

type serverSession struct {
	r            io.Reader
	w            io.Writer
	recv         chan []byte
	serverCookie []byte
	clientCookie []byte
}

const (
	stServerStart = iota
	stServerAcceptClient
	stServerHelloDone
	stServerEnd
)

func server(r io.Reader, w io.Writer) error {
	sess := serverSession{
		r:    r,
		w:    w,
		recv: make(chan []byte),
	}
	st := stServerStart

	go func() {
		for {
			buf := make([]byte, 32)
			n, err := r.Read(buf)
			if err != nil {
				break
			}
			sess.recv <- buf[:n]
		}
	}()

	for {
		switch st {
		case stServerStart:
			nextSt, err := sess.onStart()
			if err != nil {
				return err
			}
			st = nextSt

		case stServerAcceptClient:
			nextSt, err := sess.onAcceptClient()
			if err != nil {
				return err
			}
			st = nextSt

		case stServerHelloDone:
			nextSt, err := sess.onHelloDone()
			if err != nil {
				return err
			}
			st = nextSt

		case stServerEnd:
			log.Print("<server> Finished")
			return nil
		}
	}
}

// C -> S:  ClientHello | cookie
func (sess *serverSession) onStart() (int, error) {
	for {
		msg := <-sess.recv

		if msg[0] == tagClientHello {
			sess.clientCookie = msg[1:]
			return stServerAcceptClient, nil
		}
	}
}

// S -> C:  ServerHello | cookie
// C -> S:  Auth | S.cookie
func (sess *serverSession) onAcceptClient() (int, error) {
	sess.serverCookie = []byte("red")

	serverMsg := append([]byte{tagServerHello}, sess.serverCookie...)
retry:
	go sess.w.Write(serverMsg)
	timeout := time.Tick(time.Second)

	for {
		var msg []byte
		select {
		case <-timeout:
			log.Print("<server> Auth timeout")
			goto retry
		case msg = <-sess.recv:
		}

		if msg[0] == tagAuth {
			respCookie := msg[1:]
			if !bytes.Equal(respCookie, sess.serverCookie) {
				log.Printf("<server> invalid client response %q, want %q", respCookie, sess.serverCookie)
				return stServerEnd, nil
			}
			return stServerHelloDone, nil
		}
	}
}

// S -> C:  Auth | C.cookie
// C -> S:  Ack
func (sess *serverSession) onHelloDone() (int, error) {
	serverMsg := append([]byte{tagAuth}, sess.clientCookie...)
retry:
	go sess.w.Write(serverMsg)
	timeout := time.Tick(time.Second)

	for {
		var msg []byte
		select {
		case <-timeout:
			log.Print("<server> Ack timeout")
			goto retry
		case msg = <-sess.recv:
		}

		if msg[0] == tagAck {
			return stServerEnd, nil
		}
	}
}

// Client ---------------------------------------------------------------------

type clientSession struct {
	r            io.Reader
	w            io.Writer
	recv         chan []byte
	clientCookie []byte
	serverCookie []byte
}

const (
	stClientStart = iota
	stClientHelloDone
	stClientAuthDone
	stClientEnd
)

func client(r io.Reader, w io.Writer) error {
	sess := clientSession{
		r:    r,
		w:    w,
		recv: make(chan []byte),
	}
	st := stClientStart

	go func() {
		for {
			buf := make([]byte, 32)
			n, err := r.Read(buf)
			if err != nil {
				break
			}
			sess.recv <- buf[:n]
		}
	}()

	for {
		switch st {
		case stClientStart:
			nextSt, err := sess.onStart()
			if err != nil {
				return err
			}
			st = nextSt

		case stClientHelloDone:
			nextSt, err := sess.onHelloDone()
			if err != nil {
				return err
			}
			st = nextSt

		case stClientAuthDone:
			nextSt, err := sess.onAuthDone()
			if err != nil {
				return err
			}
			st = nextSt

		case stClientEnd:
			log.Print("<client> Finished")
			return nil
		}
	}
}

// C -> S:  ClientHello | cookie
// S -> C:  ServerHello | cookie
func (sess *clientSession) onStart() (int, error) {
	sess.clientCookie = []byte("blue")

	clientMsg := append([]byte{tagClientHello}, sess.clientCookie...)
retry:
	go sess.w.Write(clientMsg)
	timeout := time.After(time.Second)

	for {
		var msg []byte
		select {
		case <-timeout:
			log.Print("<client> ServerHello timeout")
			goto retry
		case msg = <-sess.recv:
		}

		if msg[0] == tagServerHello {
			sess.serverCookie = msg[1:]
			return stClientHelloDone, nil
		}
	}
}

// C -> S:  Auth | S.cookie
// S -> C:  Auth | C.cookie
func (sess *clientSession) onHelloDone() (int, error) {
	clientMsg := append([]byte{tagAuth}, sess.serverCookie...)
retry:
	go sess.w.Write(clientMsg)
	timeout := time.After(time.Second)

	for {
		var msg []byte
		select {
		case <-timeout:
			log.Print("<client> Auth timeout")
			goto retry
		case msg = <-sess.recv:
		}

		if msg[0] == tagAuth {
			respCookie := msg[1:]
			if !bytes.Equal(respCookie, sess.clientCookie) {
				log.Printf("<client> invalid server response %q, want %q", respCookie, sess.clientCookie)
				return stClientEnd, nil
			}
			return stClientAuthDone, nil
		}
	}
}

// C -> S:  Ack
func (sess *clientSession) onAuthDone() (int, error) {
	go sess.w.Write([]byte{tagAck})
	go sess.w.Write([]byte{tagAck})
	go sess.w.Write([]byte{tagAck})
	go sess.w.Write([]byte{tagAck})
	go sess.w.Write([]byte{tagAck})
	return stClientEnd, nil
}
