package main

import (
	"fmt"
	"os"

	"github.com/libp2p/go-reuseport"
)

func main() {
	if err := run(os.Args[1]); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(addr string) error {
	conn, err := reuseport.ListenPacket("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, 64)

	for {
		n, peer, err := conn.ReadFrom(buf)
		if err != nil {
			return err
		}

		fmt.Printf("From %s - (%d) %q\n", peer, n, buf[:n])
	}

	return nil
}
