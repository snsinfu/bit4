package main

import (
	"fmt"
	"net"
	"os"
	"time"
)

const (
	bufferSize   = 1472
	messageDelay = 2 * time.Second
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

	messages := [][]byte{
		[]byte("The quick brown fox jumps over the lazy dog"),
		[]byte("Lorem ipsum dolor sit amet"),
	}

	for i, msg := range messages {
		if i != 0 {
			time.Sleep(messageDelay)
		}
		if _, err := conn.Write(msg); err != nil {
			return err
		}
	}

	buf := make([]byte, bufferSize)

	for i := 0; i < 2; i++ {
		n, err := conn.Read(buf)
		if err != nil {
			return err
		}
		fmt.Printf("[client] response: '%s'\n", buf[:n])
	}

	return nil
}
