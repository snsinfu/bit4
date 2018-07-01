package main

import (
	"log"
	"net"
	"time"
)

func main() {
	ln, err := net.Listen("tcp", "localhost:9999")
	if err != nil {
		log.Fatal(err)
	}

	// ERROR
	// go waitAndInvoke(time.After(3*time.Second), ln.Close)

	// OK
	go waitAndInvoke(time.After(3*time.Second), func() { ln.Close() })

	ln.Accept()
}

func waitAndInvoke(ch <-chan time.Time, callback func()) {
	<-ch
	callback()
}
