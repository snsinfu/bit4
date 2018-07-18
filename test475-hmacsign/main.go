package main

import (
	"fmt"
	"time"

	"./sign"
)

func main() {
	msg := []byte("fall leaves as soon as leaves fall")
	key := []byte("correct horse battery staple")
	packet := sign.Sign(msg, key)

	if time.Now().Second() % 2 == 0 {
		copy(packet[40:], []byte("the quick brown fox jumps over the"))
	}

	data, ok := sign.Verify(packet, key)
	if !ok {
		fmt.Println("[ERROR] Signature verification failed!")
	}
	fmt.Println(string(data))
}
