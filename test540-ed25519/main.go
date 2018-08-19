package main

import (
	"crypto/rand"
	"fmt"
	"os"
	"time"

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

	fmt.Printf("pubkey: %x\n", pub)
	fmt.Printf("privkey: %x\n", priv)
	fmt.Println()

	message := []byte("Lorem ipsum dolor sit amet")
	signature := ed25519.Sign(priv, message)

	fmt.Printf("message: (%d) %q\n", len(message), message)
	fmt.Printf("signature: (%d) %x\n", len(signature), signature)
	fmt.Println()

	// Tamper with message
	if time.Now().Unix() % 2 == 0 {
		message[0] = 'G'
	}

	fmt.Printf("received: (%d) %q\n", len(message), message)
	if ed25519.Verify(pub, message, signature) {
		fmt.Println("verification: ok")
	} else {
		fmt.Println("verification: failed")
	}

	return nil
}
