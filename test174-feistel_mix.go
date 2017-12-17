// Using a key-less Feistel network as a bijective mixing function.
//
// Here we use SHA256 as the F-function (with empty key). Generally this is
// considered a bad idea [1][2], but we are not creating a cipher but a mixing
// function for a casual use. So I believe it's okay.
//
// [1]: https://crypto.stackexchange.com/a/27860
// [2]: https://crypto.stackexchange.com/a/29924

package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
)

func main() {
	demo([]byte{0x12, 0x34, 0x56, 0x78})
	demo([]byte{0x11, 0x34, 0x56, 0x78})
	demo([]byte{0x12, 0x34, 0x56, 0x79})
}

func demo(bits []byte) {
	srcHex := hex.EncodeToString(bits)
	mix(bits, 4)
	mixHex := hex.EncodeToString(bits)
	mix(bits, 4)
	revHex := hex.EncodeToString(bits)

	fmt.Println(srcHex, "->", mixHex, "->", revHex)
}

func mix(bits []byte, depth int) {
	if len(bits)%2 != 0 {
		panic("len(bits) must be even")
	}

	n := len(bits) / 2
	left := bits[:n]
	right := bits[n:]
	prevRight := make([]byte, n)

	for i := 0; i < depth; i++ {
		copy(prevRight, right)
		round(right)
		xor(right, left)
		copy(left, prevRight)
	}

	copy(prevRight, right)
	copy(right, left)
	copy(left, prevRight)
}

func round(right []byte) {
	hash := sha256.Sum256(right)
	for i := range right {
		right[i] = hash[i%len(hash)]
	}
}

func xor(data, key []byte) {
	for i := range data {
		data[i] ^= key[i]
	}
}
