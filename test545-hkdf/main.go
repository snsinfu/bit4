package main

import (
	"crypto/sha256"
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test545-hkdf/hkdf"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	// A secret would be a pre-shared key or a Diffie-Hellman shared secret.
	secret := []byte{
		0xa7, 0x95, 0xbb, 0xe6, 0xc2, 0x4d, 0xab, 0xe0,
		0x9c, 0x01, 0xfa, 0xb3, 0x87, 0x39, 0x49, 0x64,
	}

	// A salt would be a session-specific random salt agreed between peers.
	salt := []byte{0x9c, 0x6f, 0xdd, 0xfc, 0x3b, 0x10, 0xc4, 0x80}

	// You may want to use HKDF to derive encryption keys and IVs.
	hk := hkdf.New(sha256.New, secret, salt)

	skey := hk.MustGenerate(16, []byte("server key"))
	ckey := hk.MustGenerate(16, []byte("client key"))
	siv := hk.MustGenerate(12, []byte("server iv"))
	civ := hk.MustGenerate(12, []byte("client iv"))

	fmt.Printf("%x %x\n", skey, siv)
	fmt.Printf("%x %x\n", ckey, civ)

	return nil
}
