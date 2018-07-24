package main

import (
	"fmt"
	"time"
)

func main() {
	ch := make(chan int, 2)

	go func() {
		for range time.NewTicker(time.Second).C {
			fmt.Println("Got", <-ch)
		}
	}()

	for i := 0; i < 10; i++ {
		select {
		case ch <- i:
			fmt.Println("Put", i)

		default:
			fmt.Println("Discarded", i)
		}
	}

	time.Sleep(5 * time.Second)
}
