package main

import (
	"fmt"
)

func main() {
	msg := TagClientHello.Encode([]byte("Hello"))
	fmt.Printf("%x\n", msg)

	data, tag := Decode(msg)
	fmt.Printf("<0x%02x> %x\n", tag, data)
}

type Tag byte

const (
	TagClientHello Tag = 0x10
	TagServerHello     = 0x11
)

func (tag Tag) Encode(data []byte) []byte {
	return append([]byte{byte(tag)}, data...)
}

func Decode(msg []byte) ([]byte, Tag) {
	return msg[1:], Tag(msg[0])
}
