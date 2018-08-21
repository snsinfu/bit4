package main

import (
	"crypto/sha256"
	"fmt"

	"github.com/snsinfu/bit4/test547-hkdf/hkdf"
)

func main() {
	secret := []byte{
		0xa7, 0x95, 0xbb, 0xe6, 0xc2, 0x4d, 0xab, 0xe0,
		0x9c, 0x01, 0xfa, 0xb3, 0x87, 0x39, 0x49, 0x64,
	}
	salt := []byte{0x9c, 0x6f, 0xdd, 0xfc, 0x3b, 0x10, 0xc4, 0x80}

	hk := hkdf.New(sha256.New, secret, salt)

	skey := hk.DeriveKey(16, []byte("server key"))
	ckey := hk.DeriveKey(16, []byte("client key"))
	siv := hk.DeriveKey(12, []byte("server iv"))
	civ := hk.DeriveKey(12, []byte("client iv"))

	fmt.Printf("%x %x\n", skey, siv)
	fmt.Printf("%x %x\n", ckey, civ)
}
