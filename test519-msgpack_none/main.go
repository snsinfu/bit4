package main

import (
	"fmt"

	"github.com/vmihailenco/msgpack"
)

type Message struct {
	Cookie []byte
}

func main() {
	fmt.Println("-- Passing nil cookie")
	roundtrip(Message{
		Cookie: nil,
	})

	fmt.Println("\n-- Passing empty cookie")
	roundtrip(Message{
		Cookie: []byte{1}[:0],
	})

	// A nil slice is serialized as a nil (0xc0) and an empty slice is
	// serialized as a bin 8 (0xc4).
}

func roundtrip(m Message) {
	msg, err := msgpack.Marshal(m)
	if err != nil {
		panic(err)
	}

	r := Message{}
	if err := msgpack.Unmarshal(msg, &r); err != nil {
		panic(err)
	}

	fmt.Printf("marshal: (%d) %q\n", len(msg), msg)

	if r.Cookie == nil {
		fmt.Println("cookie: nil")
	} else {
		fmt.Println("cookie: ", r.Cookie)
	}
}
