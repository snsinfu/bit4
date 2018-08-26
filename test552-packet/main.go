package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
	"errors"
	"fmt"
)

func main() {
	s, _ := newRawSealer()
	t, _ := newGCMSealer([]byte("0123456789abcdef"), []byte("opqrstuvwxyz"))

	msg := t.Seal([]byte("Lorem ipsum"), 0xcc)
	data, tag, _ := s.Open(msg)
	fmt.Printf("<%02x> %x\n", tag, data)
}

type Sealer interface {
	Seal(data []byte, tag byte) []byte
	Open(msg []byte) ([]byte, byte, error)
}

type rawSealer struct {
}

func newRawSealer() (Sealer, error) {
	return &rawSealer{}, nil
}

func (s *rawSealer) Seal(data []byte, tag byte) []byte {
	return append([]byte{tag}, data...)
}

func (s *rawSealer) Open(msg []byte) ([]byte, byte, error) {
	if len(msg) == 0 {
		return nil, 0, errors.New("invalid message")
	}
	return msg[1:], msg[0], nil
}

type gcmSealer struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

const (
	counterSize     = 8
	headerSize      = 1 + counterSize
	messageOverhead = headerSize + 16
	nonceSize       = 12
)

func newGCMSealer(key, iv []byte) (Sealer, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	sealer := &gcmSealer{
		gcm: gcm,
		iv:  make([]byte, 12),
	}
	copy(sealer.iv, iv)

	return sealer, nil
}

func (s *gcmSealer) Seal(data []byte, tag byte) []byte {
	s.counter++
	if s.counter == 0 {
		panic("counter wraparound")
	}

	var counterCode [counterSize]byte
	binary.LittleEndian.PutUint64(counterCode[:], s.counter)

	buf := make([]byte, len(data)+messageOverhead)
	header := buf[:headerSize]
	header[0] = tag
	copy(header[1:], counterCode[:])

	var nonce [nonceSize]byte
	copy(nonce[:], s.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	return s.gcm.Seal(header, nonce[:], data, header)
}

func (s *gcmSealer) Open(msg []byte) ([]byte, byte, error) {
	if len(msg) < messageOverhead {
		return nil, 0, errors.New("invalid message")
	}

	header := msg[:headerSize]
	tag := header[0]
	counterCode := header[1:]

	counter := binary.LittleEndian.Uint64(counterCode)
	if counter <= s.counter {
		return nil, 0, errors.New("replayed message")
	}
	s.counter = counter

	var nonce [nonceSize]byte
	copy(nonce[:], s.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	data, err := s.gcm.Open(nil, nonce[:], msg[headerSize:], header)
	if err != nil {
		return nil, 0, err
	}

	return data, tag, nil
}
