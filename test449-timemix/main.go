package main

import (
	"fmt"
	"strconv"
	"time"
)

func main() {
	for i := 0; i < 10; i++ {
		fmt.Println(makeID())
	}
}

func makeID() string {
	x := uint64(time.Now().UnixNano())
	x ^= x >> 30
	x *= 0xbf58476d1ce4e5b9
	x ^= x >> 27
	x *= 0x94d049bb133111eb
	x ^= x >> 31
	return strconv.FormatUint(x, 16)
}
