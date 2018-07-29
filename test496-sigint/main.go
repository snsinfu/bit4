package main

import (
	"fmt"
	"os"
	"os/signal"
)

var sigint = make(chan os.Signal)

func main() {
	signal.Notify(sigint, os.Interrupt)

	select {
	case <-sigint:
		fmt.Println("SIGINT")
	}
}
