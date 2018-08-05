package main

import (
	"encoding/base64"
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test516-gcm_session/gcmseq"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	key, err := base64.StdEncoding.DecodeString("CIGo2rzg3WU3qoILT2LE7EBlb+PXH1S4+H740MgAU+o=")
	if err != nil {
		return err
	}

	iv, err := base64.StdEncoding.DecodeString("iLsC+1pbIQHFG6+p")
	if err != nil {
		return err
	}

	sealer, err := gcmseq.New(key, iv)
	if err != nil {
		return err
	}

	opener, err := gcmseq.New(key, iv)
	if err != nil {
		return err
	}

	msg := sealer.Seal([]byte("Lorem ipsum dolor sit amet"), 0x1234)

	fmt.Printf("message: (%d) %x\n", len(msg), msg)

	plaintext, tag, err := opener.Open(msg)
	if err != nil {
		return err
	}

	fmt.Printf("plaintext: (%d) %q\n", len(plaintext), plaintext)
	fmt.Printf("tag: 0x%x\n", tag)

	return nil
}
