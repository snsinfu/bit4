package main

import (
	"crypto/aes"
	"encoding/hex"
	"fmt"
	"log"
)

func main() {
	// Small change in input results in dramatic change in output.
	fmt.Println(psrHex("00000000000000000000000000000000"))
	fmt.Println(psrHex("00000000000000000000000000000001"))
	fmt.Println(psrHex("00000000100000000000000000000000"))
}

func psrHex(hexSrc string) string {
	src, err := hex.DecodeString(hexSrc)
	if err != nil {
		log.Fatal(err)
	}
	if len(src) != aes.BlockSize {
		log.Fatal("illegal input size")
	}

	key := [16]byte{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	}
	block, err := aes.NewCipher(key[:])
	if err != nil {
		log.Fatal(err)
	}

	var dst [16]byte
	block.Encrypt(dst[:], src)
	return hex.EncodeToString(dst[:])
}
