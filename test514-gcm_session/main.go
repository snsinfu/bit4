package main

// AES-GCM communication with authenticated counter

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/base64"
	"encoding/binary"
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
	key, err := base64.StdEncoding.DecodeString("97jfsGxM7w+3qVMFKRiX+uNYMQOuHIaXPTIhZ50r1f4=")
	if err != nil {
		return err
	}

	iv, err := base64.StdEncoding.DecodeString("KMH4NEuJF/XJXhEU")
	if err != nil {
		return err
	}

	sealer, err := newSealer(key, iv)
	if err != nil {
		return err
	}

	opener, err := newOpener(key, iv)
	if err != nil {
		return err
	}

	msg := sealer.seal([]byte("The quick brown fox jumps over the lazy dog."), 0x1234)

	fmt.Printf("(%d) %x\n", len(msg), msg)

	data, tag, err := opener.open(msg)
	if err != nil {
		return err
	}

	fmt.Printf("tag = 0x%x\n", tag)
	fmt.Printf("data = (%d) %q\n", len(data), data)

	return nil
}

const (
	minMessageSize = 26
)

type countedSealer struct {
	aead    cipher.AEAD
	iv      []byte
	counter uint64
}

func newSealer(key, iv []byte) (*countedSealer, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	aead, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	if len(iv) != aead.NonceSize() {
		return nil, errors.New("bad IV length")
	}

	sealer := &countedSealer{
		aead: aead,
		iv:   iv,
	}
	return sealer, nil
}

func (sealer *countedSealer) seal(data []byte, tag uint16) []byte {
	sealer.counter++

	if sealer.counter == 0 {
		panic("counter wraparound")
	}

	var counterCode [8]byte
	binary.BigEndian.PutUint64(counterCode[:], sealer.counter)

	buf := make([]byte, len(data)+26)
	header := buf[:10]
	binary.BigEndian.PutUint16(header, tag)
	copy(header[2:], counterCode[:])

	var nonce [12]byte
	copy(nonce[:], sealer.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	return sealer.aead.Seal(header, nonce[:], data, header)
}

type countedOpener struct {
	aead    cipher.AEAD
	iv      []byte
	counter uint64
}

func newOpener(key, iv []byte) (*countedOpener, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	aead, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	if len(iv) != aead.NonceSize() {
		return nil, errors.New("bad IV length")
	}

	opener := &countedOpener{
		aead: aead,
		iv:   iv,
	}
	return opener, nil
}

func (opener *countedOpener) open(msg []byte) ([]byte, uint16, error) {
	if len(msg) < minMessageSize {
		return nil, 0, errors.New("invalid message format")
	}

	header := msg[:10]
	tag := binary.BigEndian.Uint16(header)
	counterCode := header[2:]

	counter := binary.BigEndian.Uint64(counterCode)
	if counter <= opener.counter {
		return nil, 0, errors.New("replayed")
	}
	opener.counter = counter

	var nonce [12]byte
	copy(nonce[:], opener.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	data, err := opener.aead.Open(nil, nonce[:], msg[10:], header)
	if err != nil {
		return nil, 0, err
	}

	return data, tag, nil
}
