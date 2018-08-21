package hkdf

import (
	"crypto/hmac"
	"hash"
)

// PRK is a pseudo-random key used to derive other keys.
type PRK struct {
	key  []byte
	hash func() hash.Hash
}

// New generates a PRK from secret keying material and salt.
func New(hash func() hash.Hash, secret, salt []byte) PRK {
	if len(salt) == 0 {
		salt = make([]byte, hash().Size())
	}
	mac := hmac.New(hash, salt)
	mac.Write(secret)
	return PRK{key: mac.Sum(nil), hash: hash}
}

// DeriveKey generates an n-octet key from PRK and arbitrary context info. It
// panics if n is larger than 255*HashSize.
func (prk *PRK) DeriveKey(n int, info []byte) []byte {
	mac := hmac.New(prk.hash, prk.key)

	if n > 255*mac.Size() {
		panic("requested key is too large")
	}

	buf := make([]byte, roundUp(n, mac.Size()))
	prev := buf[:0]
	counter := byte(0)

	for m := 0; m < n; {
		counter++
		counterArr := [1]byte{counter}

		mac.Reset()
		mac.Write(prev)
		mac.Write(info)
		mac.Write(counterArr[:])
		sum := mac.Sum(buf[:m])

		prev = buf[m:len(sum)]
		m = len(sum)
	}

	return buf[:n]
}

// roundUp rounds n to the next multiple of m.
func roundUp(n, m int) int {
	return (n + m - 1) / m * m
}
