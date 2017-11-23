package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/sha512"
	"encoding/hex"
	"fmt"
	"os"

	"github.com/pkg/errors"
	"golang.org/x/crypto/pbkdf2"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func run() error {

	src := []byte("the quick brown fox jumps over the lazy dog")
	password := []byte("correct horse battery staple")
	salt := make([]byte, 16)
	if _, err := rand.Read(salt); err != nil {
		return errors.Wrap(err, "Failed to generate a salt")
	}

	ci, err := crypt(src, password, salt)
	if err != nil {
		return err
	}

	for i := range src {
		src[i] = 0
	}
	for i := range password {
		password[i] = 0
	}

	fmt.Println(ci)

	return nil
}

type ciphertext struct {
	Code, Salt, IV []byte
}

func crypt(src, password, salt []byte) (*ciphertext, error) {
	key := pbkdf2.Key(password, salt, 65536, 32, sha512.New)

	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, errors.Wrap(err, "Failed to create a cipher")
	}

	iv := make([]byte, block.BlockSize())
	if _, err := rand.Read(iv); err != nil {
		return nil, errors.Wrap(err, "Failed to generate an IV")
	}

	code := make([]byte, len(src))
	stream := cipher.NewCTR(block, iv)
	stream.XORKeyStream(code, src)

	ci := ciphertext{
		Code: code,
		Salt: salt,
		IV:   iv,
	}
	return &ci, nil
}

func (ci *ciphertext) String() string {
	str := hex.EncodeToString(ci.Salt) + "_"
	str += hex.EncodeToString(ci.IV) + "_"
	str += hex.EncodeToString(ci.Code)
	return str
}
