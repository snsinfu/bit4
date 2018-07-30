package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"fmt"
	"io"
	"os"
)

func main() {
	key := []byte{
		0x63, 0xdf, 0xde, 0x2b, 0xc7, 0x62, 0x29, 0x33,
		0x1c, 0xbb, 0xfc, 0x01, 0xf8, 0xea, 0x05, 0xff,
		0x0f, 0x9b, 0x58, 0xc0, 0x6b, 0xce, 0x57, 0x1f,
		0x7c, 0x53, 0x5d, 0x66, 0xb0, 0x12, 0xf1, 0x14,
	}
	original := []byte("the quick brown fox jumps over the lazy dog")

	block, err := aes.NewCipher(key)
	if err != nil {
		fmt.Println("error:", err.Error())
		os.Exit(1)
	}

	nonce := make([]byte, 12)
	if _, err := io.ReadFull(rand.Reader, nonce); err != nil {
		fmt.Println("error:", err.Error())
		os.Exit(1)
	}

	aesgcm, err := cipher.NewGCM(block)
	if err != nil {
		fmt.Println("error:", err.Error())
		os.Exit(1)
	}

	ciphertext := aesgcm.Seal(nil, nonce, original, nil)

	plaintext, err := aesgcm.Open(nil, nonce, ciphertext, nil)
	if err != nil {
		panic(err.Error())
	}

	fmt.Printf("Ciphertext %x\n", ciphertext)
	fmt.Printf("Plaintext %q\n", plaintext)
}
