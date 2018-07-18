package sign

import (
	"crypto/hmac"
	"crypto/sha256"
	"crypto/rand"
)

const (
	hashSize  = 32
	nonceSize = 8
)

// Sign signs data with given key using HMAC-SHA256 and random nonce.
func Sign(data, key []byte) []byte {
	packet := make([]byte, hashSize+nonceSize+len(data))

	rand.Read(packet[hashSize:hashSize+nonceSize])
	copy(packet[hashSize+nonceSize:], data)

	hmac := hmac.New(sha256.New, key)
	hmac.Write(packet[hashSize:])
	copy(packet[:hashSize], hmac.Sum(nil))

	return packet
}

// Verify verifies signed data for integrity.
func Verify(packet, key []byte) ([]byte, bool) {
	if len(packet) < hashSize + nonceSize {
		return nil, false
	}
	data := packet[hashSize+nonceSize:]

	mac := hmac.New(sha256.New, key)
	mac.Write(packet[hashSize:])
	if !hmac.Equal(mac.Sum(nil), packet[:hashSize]) {
		return data, false
	}

	return data, true
}
