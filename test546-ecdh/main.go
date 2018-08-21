package main

import (
	"crypto/rand"
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test546-ecdh/ecdh"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	dh1, err := ecdh.New(rand.Reader)
	if err != nil {
		return err
	}

	dh2, err := ecdh.New(rand.Reader)
	if err != nil {
		return err
	}

	secret1, err := dh1.Secret(dh2.Public())
	if err != nil {
		return err
	}

	secret2, err := dh2.Secret(dh1.Public())
	if err != nil {
		return err
	}

	fmt.Printf("%x\n", secret1)
	fmt.Printf("%x\n", secret2)

	return nil
}
