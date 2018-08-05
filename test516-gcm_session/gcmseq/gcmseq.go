package gcmseq

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
	"errors"
)

const (
	messageOverhead = 26
)

// ErrInvalidIV is returned by New if the size of an IV is invalid.
var ErrInvalidIV = errors.New("invalid IV size")

// ErrInvalidMessage is returned by Open if the format of a message is invalid.
var ErrInvalidMessage = errors.New("invalid message format")

// ErrReplayAttack is returned by Open if the counter value of a message is
// not greater than that of a previously decrypted one.
var ErrReplayAttack = errors.New("message is replayed")

// Cipher encrypts or decrypts a series of messages preventing replay attacks.
type Cipher struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

// New creates a Cipher with given AES key and IV. The IV is used to derive a
// nonce for each message and must be 12 bytes long.
func New(key, iv []byte) (*Cipher, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	if len(iv) != 12 {
		return nil, ErrInvalidIV
	}

	cipher := &Cipher{
		gcm:     gcm,
		iv:      make([]byte, 12),
		counter: 0,
	}
	copy(cipher.iv, iv)

	return cipher, nil
}

// Seal encrypts data and prepends given tag and counter values as a header in
// big endian format. The header is authenticated by GCM.
func (c *Cipher) Seal(data []byte, tag uint16) []byte {
	c.counter++
	if c.counter == 0 {
		panic("counter wraparound")
	}

	var counterCode [8]byte
	binary.BigEndian.PutUint64(counterCode[:], c.counter)

	buf := make([]byte, len(data)+messageOverhead)
	header := buf[:10]
	binary.BigEndian.PutUint16(header[:2], tag)
	copy(header[2:], counterCode[:])

	var nonce [12]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	return c.gcm.Seal(header, nonce[:], data, header)
}

// Open decodes and decrypts a message created by Seal.
func (c *Cipher) Open(msg []byte) ([]byte, uint16, error) {
	if len(msg) < messageOverhead {
		return nil, 0, ErrInvalidMessage
	}

	header := msg[:10]
	tag := binary.BigEndian.Uint16(header)
	counterCode := header[2:]

	counter := binary.BigEndian.Uint64(counterCode)
	if counter <= c.counter {
		return nil, 0, ErrReplayAttack
	}
	c.counter = counter

	var nonce [12]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	data, err := c.gcm.Open(nil, nonce[:], msg[10:], header)
	if err != nil {
		return nil, 0, err
	}

	return data, tag, nil
}
