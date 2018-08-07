package main

import (
	"fmt"

	"github.com/vmihailenco/msgpack"
)

func main() {
	type message struct {
		Cookie []byte
	}

	msg, err := msgpack.Marshal(message{
		Cookie: []byte{1, 2, 3, 4},
	})
	if err != nil {
		panic(err)
	}

	var m message
	if err := msgpack.Unmarshal(msg, &m); err != nil {
		panic(err)
	}

	fmt.Println(&msg[10])
	fmt.Println(&m.Cookie[0])
}
