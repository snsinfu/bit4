package main

import "fmt"

func main() {
	keys := []uint8{0x12, 0x34, 0x56, 0x78}
	plaintext := uint16(0x1234)
	ciphertext := feistel(plaintext, keys)

	fmt.Printf("%x\n", plaintext)
	fmt.Printf("%x\n", ciphertext)
	fmt.Printf("%x\n", feistel(ciphertext, reverse(keys)))
}

func reverse(keys []uint8) []uint8 {
	rev := make([]uint8, len(keys))
	for i, key := range keys {
		rev[len(rev) - i - 1] = key
	}
	return rev
}

func feistel(src uint16, keys []uint8) uint16 {
	left := uint8(src >> 8)
	right := uint8(src & 0xFF)
	for _, key := range keys {
		nextLeft := right
		right = left ^ round(right, key)
		left = nextLeft
	}
	return uint16(right) << 8 | uint16(left)
}

func round(input, key uint8) uint8 {
	return (input * key) ^ key
}
