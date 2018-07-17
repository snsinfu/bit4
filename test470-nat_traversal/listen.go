package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	if err := run(os.Args[1]); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(s string) error {
	addr, err := net.ResolveUDPAddr("udp4", s)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp4", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, 64)

	for {
		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		fmt.Printf("From %s - (%d) %q\n", peer, n, buf[:n])
	}

	return nil
}
