package gcmseq

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
)

const (
	messageOverhead = 26
)

// Sealer encrypts series of messages with authorized counter.
type Sealer struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

// NewSealer creates a Sealer with given AES key and IV. The IV is used to
// derive a nonce for each message encryption and must be 12-bytes long.
func NewSealer(key, iv []byte) (*Sealer, error) {
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

	sealer := &Sealer{
		gcm:     gcm,
		iv:      make([]byte, 12),
		counter: 0,
	}
	copy(sealer.iv, iv)

	return sealer, nil
}

// Seal encrypts data and prepends given tag and counter values as a header in
// big endian format. The header is authenticated by GCM.
func (sealer *Sealer) Seal(data []byte, tag uint16) []byte {
	sealer.counter++
	if sealer.counter == 0 {
		panic("counter wraparound")
	}

	var counterCode [8]byte
	binary.BigEndian.PutUint64(counterCode[:], sealer.counter)

	buf := make([]byte, len(data)+messageOverhead)
	header := buf[:10]
	binary.BigEndian.PutUint16(header[:2], tag)
	copy(header[2:], counterCode[:])

	var nonce [12]byte
	copy(nonce[:], sealer.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	return sealer.gcm.Seal(header, nonce[:], data, header)
}
