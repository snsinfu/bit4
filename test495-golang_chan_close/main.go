package main

import (
	"fmt"
	"time"
)

func main() {
	signalCh := make(chan bool)
	syncCh := make(chan bool)

	go func() {
		select {
		case sig := <-signalCh:
			fmt.Println("Got a signal", sig)
		}
		close(syncCh)
	}()

	time.Sleep(time.Second)
	close(signalCh)
	<-syncCh
}
