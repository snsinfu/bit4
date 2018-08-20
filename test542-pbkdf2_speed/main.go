package main

import (
	"crypto/sha256"

	"golang.org/x/crypto/pbkdf2"
	"golang.org/x/crypto/scrypt"
)

func main() {
}

func makeKeyPBKDF2(password string, iterCount int) []byte {
	salt := []byte{}
	keySize := 32
	return pbkdf2.Key([]byte(password), salt, iterCount, keySize, sha256.New)
}

func makeKeyScrypt(password string, iterCount int) []byte {
	salt := []byte{}
	keySize := 32
	key, _ := scrypt.Key([]byte(password), salt, iterCount, 8, 1, keySize)
	return key
}
