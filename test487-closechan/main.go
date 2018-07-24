package main

import (
	"fmt"
	"time"
)

func main() {
	closer := make(chan bool)
	joiner := make(chan bool)

	go tick("A", closer, joiner)
	go tick("B", closer, joiner)

	time.Sleep(2 * time.Second)

	close(closer)

	<-joiner
	<-joiner
}

func tick(name string, closer <-chan bool, joiner chan<- bool) {
	ticker := time.NewTicker(time.Second)

Loop:
	for {
		select {
		case <-ticker.C:
			fmt.Println("Tick", name)

		case <-closer:
			break Loop
		}
	}

	fmt.Println("OK", name)

	joiner <- true
}
