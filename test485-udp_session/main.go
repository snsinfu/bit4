package main

import (
	"errors"
	"log"
	"net"
	"sync"
	"sync/atomic"
	"time"
)

const (
	listenAddr     = ":1111"
	bufferSize     = 1480
	sessionTimeout = 10 * time.Second
)

type session struct {
	conn   *net.UDPConn
	peer   *net.UDPAddr
	reader chan []byte
	closer chan bool
	idle   int32
}

func main() {
	if err := run(); err != nil {
		log.Fatal("error:", err)
	}
}

func run() error {
	addr, err := net.ResolveUDPAddr("udp", listenAddr)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}

	buf := make([]byte, bufferSize)
	sessions := map[string]*session{}
	sessionsLock := sync.Mutex{}

	for {
		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		peerID := peer.String()

		sessionsLock.Lock()
		sess, ok := sessions[peerID]
		if !ok {
			sess = &session{
				conn:   conn,
				peer:   peer,
				reader: make(chan []byte, 2),
				closer: make(chan bool),
				idle:   0,
			}
			sessions[peerID] = sess

			log.Print("Start session: ", peerID)

			go func() {
				sess.main()

				sessionsLock.Lock()
				delete(sessions, peerID)
				sessionsLock.Unlock()

				log.Print("Close session: ", peerID)
			}()
		}
		sessionsLock.Unlock()

		msg := make([]byte, n)
		copy(msg, buf)
		sess.reader <- msg
	}
}

func (sess *session) main() error {
	go func() {
		for range time.NewTicker(sessionTimeout).C {
			if atomic.AddInt32(&sess.idle, 1) == 2 {
				sess.closer <- true
				break
			}
		}
	}()

	for {
		msg, err := sess.read()
		if err != nil {
			return err
		}

		if err := sess.write(msg); err != nil {
			return err
		}
	}
}

func (sess *session) read() ([]byte, error) {
	select {
	case msg := <-sess.reader:
		atomic.StoreInt32(&sess.idle, 0)
		return msg, nil

	case <-sess.closer:
	}
	return nil, errors.New("EOF")
}

func (sess *session) write(msg []byte) error {
	_, err := sess.conn.WriteToUDP(msg, sess.peer)
	return err
}
