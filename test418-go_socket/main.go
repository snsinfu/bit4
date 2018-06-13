package main

import (
	"fmt"
	"net"
	"os"
)

const (
	address = "127.0.0.1:12345"
	message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	addr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	go func() {
		dial, err := net.Dial("udp", address)
		if err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			return
		}

		if _, err := dial.Write([]byte(message)); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			return
		}

		fmt.Println("[send] OK")
	}()

	buf := make([]byte, 1024)

	n, err := conn.Read(buf)
	if err != nil {
		return err
	}

	fmt.Println("[recv] OK:", string(buf[:n]))

	return nil
}
