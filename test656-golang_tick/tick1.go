package main

import (
	"fmt"
	"time"
)

func main() {
	// No instant first tick
	// https://github.com/golang/go/issues/17601
	for range time.Tick(10*time.Second) {
		fmt.Println("tick")
	}
}
