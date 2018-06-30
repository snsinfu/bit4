package main

import (
	"crypto/rand"
	"encoding/hex"
	"fmt"

	"github.com/rs/xid"
	"github.com/satori/go.uuid"
)

func main() {
	fmt.Println(xid.New())
	fmt.Println(uuid.NewV4())
	fmt.Println(random())
}

func random() string {
	r := make([]byte, 16)
	if _, err := rand.Read(r); err != nil {
		panic(err)
	}
	return hex.EncodeToString(r)
}
