package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"io"
	"os"
	"strconv"

	"golang.org/x/crypto/hkdf"
)

func main() {
	keyLen, err := intEnv("KEYLEN", 10)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	secret := []byte("HMAC-based Extract-and-Expand Key Derivation Function")
	salt := []byte("HKDF")
	key := deriveKey(secret, salt, keyLen)

	fmt.Println(hex.EncodeToString(key))
}

func intEnv(key string, def int) (int, error) {
	if str, ok := os.LookupEnv(key); ok {
		val, err := strconv.Atoi(str)
		if err != nil {
			return def, err
		}
		return val, nil
	}
	return def, nil
}

func deriveKey(secret, salt []byte, size int) []byte {
	kdf := hkdf.New(sha256.New, secret, salt, []byte{})
	key := make([]byte, size)
	if _, err := io.ReadFull(kdf, key); err != nil {
		panic(err)
	}
	return key
}
