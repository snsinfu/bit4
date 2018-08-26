package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
	"errors"
	"fmt"
)

func main() {
	key := []byte{
		0x69, 0xc2, 0xf2, 0xd9, 0x5d, 0x18, 0x0d, 0x1f,
		0xcd, 0x46, 0x6f, 0xf9, 0x90, 0x76, 0x9a, 0xdc,
	}
	iv := []byte{
		0x19, 0x4b, 0xb4, 0xc1, 0x16, 0x89, 0xd6, 0x61,
		0x60, 0x01, 0x9c, 0x57,
	}

	sealer, _ := NewCipher(key, iv)
	opener, _ := NewCipher(key, iv)

	// Build an encrypted message
	msg := sealer.Seal([]byte("Lorem ipsum"), 0x89)
	fmt.Printf("Sealed: %x\n", msg)

	// Inspect the tag part
	data, tag, _ := DetachTag(msg)
	fmt.Printf("Detached: <x%02x> %x\n", tag, data)

	// Open the message
	data, tag, _ = opener.Open(msg)
	fmt.Printf("Opened: <x%02x> %q\n", tag, data)

	// Build a tagged plaintext message
	msg = AttachTag([]byte("Lorem ipsum"), 0x89)
	fmt.Printf("Attached: %q\n", msg)

	// Deconstruct the message
	data, tag, _ = DetachTag(msg)
	fmt.Printf("Detached: <x%02x> %q\n", tag, data)
}

type Cipher struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

const (
	counterSize     = 8
	headerSize      = counterSize + 1
	messageOverhead = headerSize + 16
	nonceSize       = 12
)

func AttachTag(data []byte, tag byte) []byte {
	return append([]byte{tag}, data...)
}

func DetachTag(msg []byte) ([]byte, byte, error) {
	if len(msg) == 0 {
		return nil, 0, errors.New("invalid message")
	}
	return msg[1:], msg[0], nil
}

func NewCipher(key, iv []byte) (*Cipher, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	sealer := &Cipher{
		gcm: gcm,
		iv:  make([]byte, nonceSize),
	}
	copy(sealer.iv, iv)

	return sealer, nil
}

func (c *Cipher) Seal(data []byte, tag byte) []byte {
	c.counter++
	if c.counter == 0 {
		panic("counter wraparound")
	}

	var counterCode [counterSize]byte
	binary.LittleEndian.PutUint64(counterCode[:], c.counter)

	buf := make([]byte, len(data)+messageOverhead)
	header := buf[:headerSize]
	header[0] = tag
	copy(header[1:], counterCode[:])

	var nonce [nonceSize]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	return c.gcm.Seal(header, nonce[:], data, header)
}

func (c *Cipher) Open(msg []byte) ([]byte, byte, error) {
	if len(msg) < messageOverhead {
		return nil, 0, errors.New("invalid message")
	}

	header := msg[:headerSize]
	tag := header[0]
	counterCode := header[1:]

	counter := binary.LittleEndian.Uint64(counterCode)
	if counter <= c.counter {
		return nil, 0, errors.New("replayed message")
	}
	c.counter = counter

	var nonce [nonceSize]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	data, err := c.gcm.Open(nil, nonce[:], msg[headerSize:], header)
	if err != nil {
		return nil, 0, err
	}

	return data, tag, nil
}
