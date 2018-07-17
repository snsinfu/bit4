package main

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

	msg := []byte("fall leaves as soon as leaves fall")

	if _, err := conn.Write(msg); err != nil {
		return err
	}

	return nil
}
