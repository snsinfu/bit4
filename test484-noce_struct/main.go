package main

import (
	"fmt"
	"time"
)

func main() {
	nd := NonceData{
		Nonce: time.Now().Unix(),
		Data:  []byte("the quick brown fox jumps over the lazy dog"),
	}
	fmt.Printf("%d %q\n", nd.Nonce, nd.Data)
}
