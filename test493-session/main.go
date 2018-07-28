package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/binary"
	"log"
	"net"
	"time"

	"github.com/snsinfu/go-taskch"
)

const (
	listenAddress = ":12345"
	bufferSize    = 1472
)

var signKey = []byte("sign key")

func main() {
	tasks := taskch.New()

	tasks.Go(server)
	time.Sleep(time.Second)
	tasks.Go(client)
	tasks.Go(client)

	if err := tasks.Wait(); err != nil {
		log.Fatal("error: ", err)
	}
}

func server() error {
	addr, err := net.ResolveUDPAddr("udp", listenAddress)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, bufferSize)
	prevNonce := int64(0)

	for {
		conn.SetReadDeadline(time.Now().Add(2 * time.Second))

		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			if neterr, ok := err.(net.Error); ok && neterr.Timeout() {
				break
			}
			return err
		}

		if n < 40 {
			log.Print("Bad message")
			continue
		}

		mac := hmac.New(sha256.New, signKey)
		mac.Write(buf[32:n])
		if !hmac.Equal(mac.Sum(nil), buf[:32]) {
			log.Print("Bad signature")
			continue
		}

		nonce := int64(binary.BigEndian.Uint64(buf[32:]))
		data := buf[40:n]

		if nonce <= prevNonce {
			log.Print("Bad nonce")
			continue
		}
		prevNonce = nonce

		log.Printf("%s: Got a signed message %q with nonce %d", peer, data, nonce)
	}

	return nil
}

func client() error {
	serverAddr, err := net.ResolveUDPAddr("udp", listenAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, serverAddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, bufferSize)
	nonce := int64(0)

	messages := [][]byte{
		[]byte("Hello!"),
		[]byte("This is second message"),
		[]byte("See you"),
	}

	for _, msg := range messages {
		nonce++
		binary.BigEndian.PutUint64(buf[32:], uint64(nonce))

		copy(buf[40:], msg)
		n := 40+len(msg)

		mac := hmac.New(sha256.New, signKey)
		mac.Write(buf[32:n])
		copy(buf, mac.Sum(nil))

		if _, err := conn.Write(buf[:n]); err != nil {
			return err
		}
	}

	return nil
}
