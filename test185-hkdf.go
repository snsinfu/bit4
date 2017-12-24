package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
)

func main() {
	message := []byte("")
	key := []byte("")
	size := 5
	hash := hashSeq(message, key, size)
	fmt.Println(hex.EncodeToString(hash))
}

// hashSeq computes keyed hash of arbitrary length using HKDF-SHA256 [1].
//
// [1]: https://tools.ietf.org/html/rfc5869
func hashSeq(msg, key []byte, size int) []byte {
	var okm []byte
	var prev []byte
	var counter [1]byte
	mac := hmac.New(sha256.New, key)
	for len(okm) < size {
		counter[0]++
		mac.Reset()
		mac.Write(prev)
		mac.Write(counter[:])
		okm = append(okm, mac.Sum(nil)...)
	}
	return okm[:size]
}
