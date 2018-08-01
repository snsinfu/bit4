package main

import (
	"crypto/hmac"
	"crypto/rand"
	"crypto/sha256"
	"fmt"
	"io"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	// A and B share a secret key in advance via a secure channel
	secret := []byte{
		0x74, 0x2a, 0xa9, 0x26, 0x5d, 0xf9, 0x3e, 0x21,
		0xd0, 0x17, 0x86, 0x47, 0x54, 0x83, 0x38, 0xef,
		0x3b, 0x3b, 0xf6, 0x9c, 0x93, 0x7a, 0x82, 0xa5,
		0x37, 0x52, 0xa9, 0x1c, 0x08, 0x65, 0x17, 0xbf,
	}

	// Session starts

	// A generates random data
	randA := make([]byte, 16)
	if _, err := io.ReadFull(rand.Reader, randA); err != nil {
		return err
	}

	// B generates random data
	randB := make([]byte, 16)
	if _, err := io.ReadFull(rand.Reader, randB); err != nil {
		return err
	}

	// A and B share their random data via an insecure channel

	// Both party derives session keys from the shared random data
	mac := hmac.New(sha256.New, secret)
	mac.Write(randA)
	mac.Write(randB)
	keyA := mac.Sum(nil)

	mac.Reset()
	mac.Write(randB)
	mac.Write(randA)
	keyB := mac.Sum(nil)

	// Afterwards the keys are used to encrypt messages
	fmt.Printf("%x\n", keyA)
	fmt.Printf("%x\n", keyB)

	return nil
}
