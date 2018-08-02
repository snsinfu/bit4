package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/base64"
	"encoding/binary"
	"encoding/json"
	"errors"
	"fmt"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	key, err := base64.StdEncoding.DecodeString("hDZExTFJ0OxiHssQbTQbZr7cKZI23nQnsnYmw+SrmN4=")
	if err != nil {
		return err
	}

	iv, err := base64.StdEncoding.DecodeString("RJ3MLK26hiyJ/MPF")
	if err != nil {
		return err
	}

	block, err := aes.NewCipher(key)
	if err != nil {
		return err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return err
	}

	const (
		tyHelloMessage byte = iota
	)

	type HelloMessage struct {
		Counter uint64
		Name    string
		Text    string
	}

	// Sender encodes a message
	counter := uint64(0)
	prevCounter := counter

	for i := 0; i < 3; i++ {
		counter++

		msg := HelloMessage{
			Counter: counter,
			Name:    "alice",
			Text:    "Hello!",
		}

		fmt.Println("Sending:", msg)

		plaintext, err := json.Marshal(msg)
		if err != nil {
			return err
		}

		nonce := make([]byte, 12)
		binary.BigEndian.PutUint64(nonce, counter)
		for i := range iv {
			nonce[i] ^= iv[i]
		}

		header := make([]byte, 13)
		header[0] = tyHelloMessage
		copy(header[1:], nonce)

		message := gcm.Seal(header, nonce, plaintext, header[:1])

		fmt.Printf("Message: (%d) %x\n", len(message), message)

		// Receiver decodes the message
		if len(message) < 13 {
			return errors.New("invalid message")
		}

		if message[0] != tyHelloMessage {
			return errors.New("unexpected message")
		}

		plaintext, err = gcm.Open(nil, message[1:13], message[13:], message[:1])
		if err != nil {
			return err
		}

		if err := json.Unmarshal(plaintext, &msg); err != nil {
			return err
		}

		if msg.Counter <= prevCounter {
			return errors.New("detected a replay attack")
		}
		prevCounter = msg.Counter

		fmt.Println("Received:", msg)
		fmt.Println()
	}

	return nil
}
