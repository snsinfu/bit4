package main

import (
	"crypto/rand"
	"fmt"
	"io"
	"os"

	"golang.org/x/crypto/curve25519"

	"github.com/snsinfu/go-taskch"
)

func main() {
	tasks := taskch.New()

	r1, w1 := io.Pipe()
	r2, w2 := io.Pipe()

	tasks.Go(func() error {
		return run("peer1", r2, w1)
	})

	tasks.Go(func() error {
		return run("peer2", r1, w2)
	})

	if err := tasks.Wait(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(name string, r io.Reader, w io.Writer) error {
	// Generate my keypair
	var privkey [32]byte
	if _, err := io.ReadFull(rand.Reader, privkey[:]); err != nil {
		return err
	}

	var pubkey [32]byte
	curve25519.ScalarBaseMult(&pubkey, &privkey)

	// Send my pubkey
	go w.Write(pubkey[:])

	// Receive peer's pubkey
	var peerPubkey [32]byte
	if _, err := io.ReadFull(r, peerPubkey[:]); err != nil {
		return err
	}

	// Derive shared secret
	var secret [32]byte
	curve25519.ScalarMult(&secret, &privkey, &peerPubkey)

	fmt.Printf("[%s] secret: %x\n", name, secret[:])

	// Now we can derive keys and IVs for symmetric cryptography from the shared
	// secret using HKDF.

	return nil
}
