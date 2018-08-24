package main

import (
	"fmt"
	"time"
)

func main() {
	tick := time.Tick(5*time.Second)

	select {
	case <-tick:
		fmt.Println("tick")
	default: // needed
	}

	fmt.Println("done")
}
