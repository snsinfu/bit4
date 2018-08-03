package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/base64"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"

	"github.com/snsinfu/go-taskch"
	"github.com/vmihailenco/msgpack"
)

const (
	tagHelloMessage byte = 0x12
)

type helloMessage struct {
	Name string `msgpack:"name"`
	Text string `msgpack:"text"`
}

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

	tasks := taskch.New()
	r, w := io.Pipe()

	tasks.Go(func() error {
		err := sender(w, key, iv)
		w.Close()
		return err
	})

	tasks.Go(func() error {
		err := receiver(r, key, iv)
		r.Close()
		return err
	})

	return tasks.Wait()
}

func sender(w io.Writer, key, iv []byte) error {
	block, err := aes.NewCipher(key)
	if err != nil {
		return err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return err
	}

	counter := uint32(0)
	buf := make([]byte, 1024)
	nonce := make([]byte, 12)

	for i := 0; i < 2; i++ {
		// Prepare a message to send
		counter++

		msg := helloMessage{
			Name: "alice",
			Text: "Hello!",
		}
		tag := tagHelloMessage

		fmt.Printf("sender: Sending message [%d] %s\n", counter, msg)

		// Encode message as MessagePack
		plaintext, err := msgpack.Marshal(msg)
		if err != nil {
			return err
		}

		fmt.Printf("sender: Plaintext: (%d) %q\n", len(plaintext), plaintext)

		// Nonce is IV xor counter
		var counterCode [4]byte
		binary.BigEndian.PutUint32(counterCode[:], counter)

		copy(nonce, iv)
		for i := range counterCode {
			nonce[i] ^= counterCode[i]
		}

		fmt.Printf("sender: Counter and nonce: [%d] %x\n", counter, nonce)

		// Prepend message tag and counter as an authenticated header
		buf[0] = tag
		copy(buf[1:], counterCode[:])

		message := gcm.Seal(buf[:5], nonce, plaintext, buf[:5])

		fmt.Printf("sender: Ciphertext: (%d) %x\n", len(message), message)

		// Send to the peer
		if _, err := w.Write(message); err != nil {
			return err
		}
	}

	return nil
}

func receiver(r io.Reader, key, iv []byte) error {
	block, err := aes.NewCipher(key)
	if err != nil {
		return err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return err
	}

	prevCounter := uint32(0)
	buf := make([]byte, 1024)
	nonce := make([]byte, 12)

	for {
		n, err := r.Read(buf)
		if err != nil {
			if err == io.EOF {
				return nil
			}
			return err
		}

		fmt.Printf("receiver: Received: (%d) %x\n", n, buf[:n])

		// Extract message tag and counter from header
		if n < 5 {
			return errors.New("invalid message")
		}

		tag := buf[0]
		counterCode := buf[1:5]
		counter := binary.BigEndian.Uint32(counterCode)

		// Nonce is IV xor counter
		copy(nonce, iv)

		for i := range counterCode {
			nonce[i] ^= counterCode[i]
		}

		fmt.Printf("receiver: Counter and nonce: [%d] %x\n", counter, nonce)

		// Decrypt the message
		plaintext, err := gcm.Open(nil, nonce, buf[5:n], buf[:5])
		if err != nil {
			return err
		}

		fmt.Printf("receiver: Plaintext: (%d) %q\n", len(plaintext), plaintext)

		// Decode the message
		if tag != tagHelloMessage {
			continue
		}

		msg := helloMessage{}
		if err := msgpack.Unmarshal(plaintext, &msg); err != nil {
			return err
		}

		// Validate counter value
		if counter <= prevCounter {
			return errors.New("detected a replay attack")
		}
		prevCounter = counter

		fmt.Printf("receiver: Received message [%d] %s\n", counter, msg)
	}
}
