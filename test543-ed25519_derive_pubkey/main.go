package main

import (
	"crypto/rand"
	"fmt"
	"os"

	//"golang.org/x/crypto/curve25519"
	"golang.org/x/crypto/ed25519"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	pub, priv, err := ed25519.GenerateKey(rand.Reader)
	if err != nil {
		return err
	}

	// No this does not work...
	/*
	var seed [32]byte
	copy(seed[:], priv.Seed())
	
	var derivedPub [32]byte
	curve25519.ScalarBaseMult(&derivedPub, &seed)
	
	fmt.Printf("%x\n", pub)
	fmt.Printf("%x\n", derivedPub[:])
	*/

	seed := priv.Seed()
	priv2 := ed25519.NewKeyFromSeed(seed)

	fmt.Printf("%x\n", pub)
	fmt.Printf("%x\n", priv2.Public())

	return nil
}
