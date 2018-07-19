package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/binary"
)

const (
	hashSize  = 32
	nonceSize = 8
)

type Signer struct {
	key []byte
}

func NewSigner(key []byte) Signer {
	return Signer{key: key}
}

func (signer Signer) Sign(data []byte, nonce uint64) []byte {
	nonceData := make([]byte, nonceSize+len(data))
	binary.BigEndian.PutUint64(nonceData, nonce)
	copy(nonceData[nonceSize:], data)

	mac := hmac.New(sha256.New, signer.key)
	mac.Write(nonceData)

	return append(mac.Sum(nil), nonceData...)
}

func (signer Signer) Verify(msg []byte, prevNonce uint64) ([]byte, uint64, bool) {
	if len(msg) < hashSize+nonceSize {
		return nil, 0, false
	}

	nonce := binary.BigEndian.Uint64(msg[hashSize:])
	if nonce <= prevNonce {
		return nil, nonce, false
	}

	mac := hmac.New(sha256.New, signer.key)
	mac.Write(msg[hashSize:])

	ok := hmac.Equal(mac.Sum(nil), msg[:hashSize])
	data := msg[hashSize+nonceSize:]

	return data, nonce, ok
}
