package main

import (
	"fmt"

	"github.com/vmihailenco/msgpack"
)

func main() {
	type Empty struct {
	}

	msg, err := msgpack.Marshal(&Empty{})
	if err != nil {
		panic(err)
	}

	fmt.Printf("(%d) %x\n", len(msg), msg)

	// It's serialized as {0x80}, a zero-element map.
}
