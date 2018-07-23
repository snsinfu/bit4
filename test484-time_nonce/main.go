package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/binary"
	"errors"
	"log"
	"net"
	"time"

	"github.com/snsinfu/go-taskch"
)

const (
	serverAddress   = ":22222"
	nonceTimeWindow = 30
)

var signKey = []byte{0x6a, 0xed, 0xc6, 0x07, 0xa5, 0xe3, 0x68, 0xd9}

func main() {
	tasks := taskch.New()

	tasks.Go(server)

	time.Sleep(time.Second)

	tasks.Go(client)
	tasks.Go(client)

	if err := tasks.Wait(); err != nil {
		log.Fatal("error:", err)
	}
}

func server() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	prevNonce := time.Now().Unix() / nonceTimeWindow
	successCount := 0

	buf := make([]byte, 128)
	for {
		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}
		log.Printf("message from %s", peer)

		msg, err := verify(buf[:n], signKey)
		if err != nil {
			log.Printf("%s: bad message: %s", peer, err)
			continue
		}

		_, nonce, err := decodeNonce(msg)
		if err != nil {
			log.Printf("%s: bad message: %s", peer, err)
			continue
		}

		if nonce <= prevNonce {
			log.Printf("%s: bad nonce: %d <= %d", peer, nonce, prevNonce)
			continue
		}
		log.Printf("%s: good nonce: %d > %d", peer, nonce, prevNonce)
		prevNonce = nonce

		resp := sign(encodeNonce([]byte("OK"), nonce), signKey)
		if _, err := conn.WriteToUDP(resp, peer); err != nil {
			return err
		}

		successCount++
		if successCount == 2 {
			break
		}
	}

	return nil
}

func client() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	nonce := time.Now().Unix() / nonceTimeWindow
	readCh := make(chan bool)

	go func() {
		ticker := time.NewTicker(time.Second)
		for {
			select {
			case <-readCh:
				return
			case <-ticker.C:
			}
			nonce++

			data := encodeNonce([]byte("Hello"), nonce)
			msg := sign(data, signKey)
			if _, err := conn.Write(msg); err != nil {
				return
			}
		}
	}()

	buf := make([]byte, 128)
	n, err := conn.Read(buf)
	if err != nil {
		return err
	}
	readCh <- true

	resp, err := verify(buf[:n], signKey)
	if err != nil {
		return err
	}

	_, respNonce, err := decodeNonce(resp)
	if err != nil {
		return err
	}

	if respNonce != nonce {
		log.Printf("bad server nonce: got %d, want %d", respNonce, nonce)
	}

	return nil
}

func sign(data, key []byte) []byte {
	mac := hmac.New(sha256.New, key)
	mac.Write(data)
	return append(mac.Sum(nil), data...)
}

func verify(msg, key []byte) ([]byte, error) {
	if len(msg) < 32 {
		return nil, errors.New("message not signed")
	}

	hash := msg[:32]
	data := msg[32:]

	mac := hmac.New(sha256.New, key)
	mac.Write(data)
	if !hmac.Equal(mac.Sum(nil), hash) {
		return data, errors.New("bad message hash")
	}

	return data, nil
}

func encodeNonce(data []byte, nonce int64) []byte {
	msg := make([]byte, 8+len(data))
	binary.BigEndian.PutUint64(msg, uint64(nonce))
	copy(msg[8:], data)
	return msg
}

func decodeNonce(msg []byte) ([]byte, int64, error) {
	if len(msg) < 8 {
		return nil, 0, errors.New("bad message")
	}
	return msg[8:], int64(binary.BigEndian.Uint64(msg)), nil
}
