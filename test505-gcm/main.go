package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"time"

	"github.com/snsinfu/go-taskch"
)

func main() {
	if err := run(); err != nil {
		fmt.Println("error:", err)
		os.Exit(1)
	}
}

func run() error {
	sec, err := makeSecret()
	if err != nil {
		return err
	}

	uplink := make(chan []byte)
	downlink := make(chan []byte)

	tasks := taskch.New()

	tasks.Go(func() error {
		return server(*sec, uplink, downlink)
	})

	tasks.Go(func() error {
		return client(*sec, downlink, uplink)
	})

	tasks.Go(func() error {
		time.Sleep(3 * time.Second)
		close(uplink)
		close(downlink)
		return nil
	})

	return tasks.Wait()
}

// sessionSecret contains secret data requied to encrypt and decrypt AES-GCM
// messages.
type sessionSecret struct {
	key []byte
	iv  []byte
}

// server
func server(sec sessionSecret, recv <-chan []byte, send chan<- []byte) error {
	aesgcm, err := makeAESGCM(sec.key)
	if err != nil {
		return err
	}

	i := 0
	for {
		msg := <-recv
		if len(msg) == 0 {
			break
		}

		// Emulate packet loss and corruption
		i++

		switch i % 5 {
		case 1:
			// Loss
			continue

		case 2:
			// Truncation
			msg = msg[:len(msg)-1]

		case 3:
			// Corruption
			msg[0] ^= 0xcc

		default:
		}

		if len(msg) < 12 {
			fmt.Println("server: Bad message")
			continue
		}

		nonce := msg[:12]
		ciphertext := msg[12:]

		plaintext, err := aesgcm.Open(nil, nonce, ciphertext, nil)
		if err != nil {
			fmt.Println("server: AEAD error:", err)
			continue
		}

		fmt.Printf("server: Got message %q nonce %x\n", plaintext, nonce)
	}

	return nil
}

// client
func client(sec sessionSecret, recv <-chan []byte, send chan<- []byte) error {
	aesgcm, err := makeAESGCM(sec.key)
	if err != nil {
		return err
	}

	for counter := uint64(0); counter < 10; counter++ {
		plaintext := []byte(fmt.Sprintf("Lorem ipsum dolor sit amet [%d]", counter))

		nonce := makeNonce(sec.iv, counter)
		ciphertext := aesgcm.Seal(nil, nonce, plaintext, nil)

		send <- append(nonce, ciphertext...)
	}

	return nil
}

// makeNonce modulates an IV with counter value.
func makeNonce(iv []byte, counter uint64) []byte {
	nonce := make([]byte, len(iv))
	binary.BigEndian.PutUint64(nonce, counter)

	for i := range iv {
		nonce[i] ^= iv[i]
	}

	return nonce
}

// makeSecret creates sessionSecret by reading sufficient random data from a
// cryptographic source.
func makeSecret() (*sessionSecret, error) {
	buf := make([]byte, 44)
	if _, err := io.ReadFull(rand.Reader, buf); err != nil {
		return nil, err
	}

	sec := &sessionSecret{
		key: buf[:32],
		iv:  buf[32:],
	}
	return sec, nil
}

// makeAESGCM creates an AES-GCM cipher.
func makeAESGCM(key []byte) (cipher.AEAD, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	return cipher.NewGCM(block)
}
