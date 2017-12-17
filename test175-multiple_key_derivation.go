// https://crypto.stackexchange.com/a/5978
// https://tools.ietf.org/html/rfc5869

package main

import (
	"crypto/hmac"
	"crypto/sha512"
	"encoding/hex"
	"fmt"

	"golang.org/x/crypto/pbkdf2"
)

func main() {
	password := []byte("correct horse battery staple")
	salt := make([]byte, 16) // stub
	masterKey := pbkdf2.Key(password, salt, 65536, 32, sha512.New)

	mac := hmac.New(sha512.New, masterKey)

	var prevKey []byte
	for i := 0; i < 10; i++ {
		counter := [1]byte{byte(i + 1)}
		mac.Reset()
		mac.Write(prevKey)
		mac.Write(counter[:])
		key := mac.Sum(nil)
		prevKey = key

		fmt.Println(hex.EncodeToString(key))
	}
}
