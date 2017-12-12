package main

import (
	"fmt"
	"log"
	"math/rand"

	"github.com/jwilder/encoding/simple8b"
)

func main() {
	enc := simple8b.NewEncoder()

	val := int64(1234567890)
	prev := val
	for i := 1; i < 100; i++ {
		val += 2 - rand.Int63n(5)
		delta := val - prev
		prev = val
		enc.Write(zigZag(delta))
	}

	code, err := enc.Bytes()
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(len(code))
	fmt.Println(code)
}

func zigZag(x int64) uint64 {
	return uint64(x<<1) ^ uint64(x>>63)
}
