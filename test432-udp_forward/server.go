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
		return fmt.Errorf("usage: server <addr>")
	}

	addr, err := net.ResolveUDPAddr("udp", os.Args[1])
	if err != nil {
		return err
	}

	fmt.Println("[server] Listening on", addr)

	return server(addr)
}

func server(addr *net.UDPAddr) error {
	sock, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer sock.Close()

	buf := make([]byte, bufferSize)

	for {
		n, peer, err := sock.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		msg := buf[:n]
		fmt.Printf("[server] (%s) '%s'\n", peer, string(msg))

		resp := []byte(fmt.Sprintf("OK; you are %s", peer))
		if _, err := sock.WriteToUDP(resp, peer); err != nil {
			return err
		}
	}
}
