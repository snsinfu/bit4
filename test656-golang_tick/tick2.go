package main

import (
	"fmt"
	"time"
)

func main() {
	tick := time.NewTicker(10*time.Second)
	for ; true; <-tick.C {
		fmt.Println("tick")
	}
}
