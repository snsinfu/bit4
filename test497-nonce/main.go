package main

import (
	"crypto/hmac"
	"crypto/rand"
	"crypto/sha256"
	"errors"
	"log"
	"net"
	"sync"
	"time"

	"github.com/snsinfu/go-taskch"
)

const (
	serverAddress = "127.0.0.1:10000"
	bufferSize    = 1472
	nonceSize     = 8
	challengeSize = 8
	macSize       = 32
	sharedKey     = "dc09a8e5757777db6c3e41ccc2539f3c"
)

type session struct {
	conn   *net.UDPConn
	client *net.UDPAddr
	recv   chan []byte
}

func main() {
	tasks := taskch.New()

	tasks.Go(server)

	time.Sleep(time.Second)

	for i := 0; i < 1000; i++ {
		tasks.Go(client)
	}

	if err := tasks.Wait(); err != nil {
		log.Print("error: ", err)
	}
}

func server() error {
	server, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", server)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, bufferSize)
	sessions := map[string]*session{}
	sessionsLock := sync.Mutex{}

	for {
		n, client, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		clientID := client.String()

		sessionsLock.Lock()
		sess, ok := sessions[clientID]
		if !ok {
			sess = &session{
				conn:   conn,
				client: client,
				recv:   make(chan []byte, 32),
			}

			go func() {
				if err := sess.main(); err != nil {
					log.Printf("%s: failed: %s", clientID, err)
				} else {
					log.Printf("%s: OK", clientID)
				}

				sessionsLock.Lock()
				delete(sessions, clientID)
				sessionsLock.Unlock()
			}()

			sessions[clientID] = sess
		}
		sessionsLock.Unlock()

		msg := make([]byte, n)
		copy(msg, buf)
		sess.recv <- msg
	}
}

func client() error {
	server, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, server)
	if err != nil {
		return err
	}
	defer conn.Close()

	// 1: Client nonce
	cnonce := make([]byte, nonceSize)
	rand.Read(cnonce)

	if _, err := conn.Write(cnonce); err != nil {
		return err
	}

	// 2: Server nonce and challenge
	buf := make([]byte, bufferSize)

	snonce := make([]byte, nonceSize)
	schal := make([]byte, challengeSize)

	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if sender.String() != server.String() {
			continue
		}

		if n != nonceSize+challengeSize {
			return errors.New("invalid server challenge")
		}

		copy(snonce, buf[:nonceSize])
		copy(schal, buf[nonceSize:])
		break
	}

	// 3: Client response and challenge
	cmac := hmac.New(sha256.New, []byte(sharedKey))
	cmac.Write(cnonce)
	cmac.Write(snonce)
	cmac.Write(schal)
	cmacSum := cmac.Sum(nil)

	cchal := make([]byte, challengeSize)
	rand.Read(cchal)

	if _, err := conn.Write(append(cmacSum, cchal...)); err != nil {
		return err
	}

	// 4: Server response
	smac := hmac.New(sha256.New, []byte(sharedKey))
	smac.Write(cnonce)
	smac.Write(snonce)
	smac.Write(cchal)
	smacSum := smac.Sum(nil)

	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if sender.String() != server.String() {
			continue
		}

		if n != macSize {
			return errors.New("invalid server response")
		}

		if !hmac.Equal(buf[:n], smacSum) {
			return errors.New("invalid server mac")
		}

		break
	}

	return nil
}

func (sess *session) main() error {
	// 1: Client nonce
	cnonce := <-sess.recv

	// 2: Server nonce
	snonce := make([]byte, nonceSize)
	rand.Read(snonce)

	schal := make([]byte, challengeSize)
	rand.Read(schal)

	if _, err := sess.conn.WriteToUDP(append(snonce, schal...), sess.client); err != nil {
		return err
	}

	// 3: Client response
	smac := hmac.New(sha256.New, []byte(sharedKey))
	smac.Write(cnonce)
	smac.Write(snonce)
	smac.Write(schal)
	smacSum := smac.Sum(nil)

	cresp := <-sess.recv
	if len(cresp) != macSize+challengeSize {
		return errors.New("invalid client response")
	}

	if !hmac.Equal(cresp[:macSize], smacSum) {
		return errors.New("invalid client mac")
	}

	cchal := cresp[macSize:]

	// 4: Server response
	cmac := hmac.New(sha256.New, []byte(sharedKey))
	cmac.Write(cnonce)
	cmac.Write(snonce)
	cmac.Write(cchal)
	cmacSum := cmac.Sum(nil)

	if _, err := sess.conn.WriteToUDP(cmacSum, sess.client); err != nil {
		return err
	}

	return nil
}
