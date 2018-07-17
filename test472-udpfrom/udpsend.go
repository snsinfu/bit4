package main

// I need to set SO_REUSEADDR to send a UDP packet with source address already
// used by a UDP service (which should be useful for a hole punching purpose).
// Upcoming Go 1.11 will allow setting socket options upon dialing, but Go 1.10
// does not provide a standard way to do so.
//
// https://github.com/golang/go/issues/9661

import (
	"fmt"
	"os"

	"github.com/libp2p/go-reuseport"
)

func main() {
	if err := run(os.Args[1], os.Args[2]); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(src, dest string) error {
	conn, err := reuseport.Dial("udp", src, dest)
	if err != nil {
		return err
	}
	defer conn.Close()

	msg := []byte("fall leaves as leaves fall")
	if _, err := conn.Write(msg); err != nil {
		return err
	}

	return nil
}
