package hkdf

import (
	"crypto/hmac"
	"errors"
	"hash"
)

// ErrTooLongKey is returned when a requested key length is too long for an
// HKDF Expand operation.
var ErrTooLongKey = errors.New("requested key length is too long")

// PRK is an HKDF pseudorandom key used to derive other crypto keys.
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

// Generate derives an n-octet key from PRK and context info. It returns an
// error if and only if n is larger than 255 * hash-size.
func (prk *PRK) Generate(n int, info []byte) ([]byte, error) {
	mac := hmac.New(prk.hash, prk.key)

	if n > 255*mac.Size() {
		return nil, ErrTooLongKey
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

	return buf[:n], nil
}

// MustGenerate derives an n-octet key from PRK and context info. It panics if
// n is larger than 255 * hash-size.
func (prk *PRK) MustGenerate(n int, info []byte) []byte {
	key, err := prk.Generate(n, info)
	if err != nil {
		panic(err)
	}
	return key
}

// roundUp rounds n to the next multiple of m.
func roundUp(n, m int) int {
	return (n + m - 1) / m * m
}
