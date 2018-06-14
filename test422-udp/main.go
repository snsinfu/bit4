package main

import (
	"fmt"
	"net"
	"os"
)

const (
	udpListenAddress = "127.0.0.1:4000"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	addr, err := net.ResolveUDPAddr("udp", udpListenAddress)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, 1)

	for {
		// How can I make it block until a message arrives without actually
		// reading it?
		//
		// - empty buffer => panic
		// - lenth 1 buffer => first octet is read, the rest is discarded

		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		msg := buf[:n]
		fmt.Printf("%s:%d -> (%d) '%s'\n", peer.IP, peer.Port, n, msg)
	}

	return nil
}
