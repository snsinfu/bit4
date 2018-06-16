package main

import (
	"fmt"
	"net"
	"os"
)

const (
	bufferSize = 1472
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	if len(os.Args) != 2 {
		return fmt.Errorf("usage: client <addr>")
	}

	addr, err := net.ResolveUDPAddr("udp", os.Args[1])
	if err != nil {
		return err
	}

	return client(addr)
}

func client(addr *net.UDPAddr) error {
	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	msg := []byte("The quick brown fox jumps over the lazy dog")
	if _, err := conn.Write(msg); err != nil {
		return err
	}

	buf := make([]byte, bufferSize)
	n, err := conn.Read(buf)
	if err != nil {
		return err
	}
	resp := buf[:n]

	fmt.Printf("[client] response: '%s'\n", resp)

	return nil
}
