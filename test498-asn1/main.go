package main

import (
	"encoding/asn1"
	"fmt"
	"log"
)

func main() {
	type Message struct {
		Type  string
		Nonce int64
		Data  []byte
	}

	msg := Message{
		Type:  "message",
		Nonce: 12345,
		Data:  []byte("Distinguished Encoding Rules (DER)"),
	}

	data, err := asn1.Marshal(msg)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("(%d) %q\n", len(data), data)

	if _, err := asn1.Unmarshal(data, &msg); err != nil {
		log.Fatal(err)
	}
}
