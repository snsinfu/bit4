package gcmseq

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
)

// Opener decrypts series of messages checking replay attack.
type Opener struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

// NewOpener creates an Opener with given AES key and IV. The IV is used to
// derive a nonce for each message decryption and must be 12-bytes long.
func NewOpener(key, iv []byte) (*Opener, error) {
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

	opener := &Opener{
		gcm:     gcm,
		iv:      make([]byte, 12),
		counter: 0,
	}
	copy(opener.iv, iv)

	return opener, nil
}

// Open decodes and decrypts a message sealed by Sealer.
func (opener *Opener) Open(msg []byte) ([]byte, uint16, error) {
	if len(msg) < messageOverhead {
		return nil, 0, ErrInvalidMessage
	}

	header := msg[:10]
	tag := binary.BigEndian.Uint16(header)
	counterCode := header[2:]

	counter := binary.BigEndian.Uint64(counterCode)
	if counter <= opener.counter {
		return nil, 0, ErrReplayAttack
	}
	opener.counter = counter

	var nonce [12]byte
	copy(nonce[:], opener.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	data, err := opener.gcm.Open(nil, nonce[:], msg[10:], header)
	if err != nil {
		return nil, 0, err
	}

	return data, tag, nil
}
