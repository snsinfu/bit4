package main

import (
	"errors"
	"fmt"
	"net"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	if len(os.Args) != 2 {
		return errors.New("usage: send <hostname>:<port>")
	}

	addr, err := net.ResolveUDPAddr("udp", os.Args[1])
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}

	messages := []string{
		"Lorem ipsum",
		"dolor sit amet",
	}

	for _, msg := range messages {
		if _, err := conn.Write([]byte(msg)); err != nil {
			return err
		}
	}

	return nil
}
