package main

import (
	"fmt"

	"github.com/snsinfu/bit4/test816-golang_chan_comm/kvs"
)

func main() {
	db := kvs.New()
	defer db.Close()

	db.Store("foo", "123")
	db.Store("bar", "456")

	fmt.Println(db.Fetch("foo"))
	fmt.Println(db.Fetch("bar"))
	fmt.Println(db.Fetch("baz"))
}
