package ecdh

import (
	"errors"
	"io"

	"golang.org/x/crypto/curve25519"
)

var ErrInvalidPublicKey = errors.New("public key must be 32-octet long")

type PrivateKey struct {
	key [32]byte
}

// New reads a new private key from a secure random source.
func New(r io.Reader) (*PrivateKey, error) {
	var priv PrivateKey
	if _, err := io.ReadFull(r, priv.key[:]); err != nil {
		return nil, err
	}
	return &priv, nil
}

// Public returns the public key paired to priv.
func (priv *PrivateKey) Public() []byte {
	var pub [32]byte
	curve25519.ScalarBaseMult(&pub, &priv.key)
	return pub[:]
}

// Secret returns a 32-octet shared secret computed from our private key and
// peer's public key.
func (priv *PrivateKey) Secret(pub []byte) ([]byte, error) {
	if len(pub) != 32 {
		return nil, ErrInvalidPublicKey
	}
	var pubKey [32]byte
	var secret [32]byte
	copy(pubKey[:], pub)
	curve25519.ScalarMult(&secret, &priv.key, &pubKey)
	return secret[:], nil
}
