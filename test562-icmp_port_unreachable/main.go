package main

// Is it possible to recover from UDP error due to ICMP Port Unreachable?
//
// -> Yes! Sending a UDP packet to closed port causes next read operation to fail (not write
// operation). This error is recoverable. At least on FreeBSD, linux and macOS.

import (
	"fmt"
	"net"
	"os"
)

const (
	serverAddress = "127.0.0.1:9999"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	// Write do not fail.
	if _, err := conn.Write([]byte("Hello")); err != nil {
		fmt.Println("Write error:", err)
	}

	// The next read fails due to ICMP Port Unreachable. (Write operation won't fail!)
	buf := make([]byte, 1472)
	if _, err := conn.Read(buf); err != nil {
		if opErr, ok := err.(*net.OpError); ok {
			fmt.Println("Read OpError:", opErr)
			fmt.Println("  Op         ->", opErr.Op)
			fmt.Println("  Net        ->", opErr.Net)
			fmt.Println("  Source     ->", opErr.Source)
			fmt.Println("  Addr       ->", opErr.Addr)
			fmt.Println("  Temporary? ->", opErr.Temporary())
		} else {
			fmt.Println("Unknown error:", err)
		}
	}

	// Now start the server and...
	serv, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer serv.Close()

	// Send a message to the failed client (the message should be queued in OS' network stack for
	// subsequent read; it's not guaranteed though).
	serv.WriteToUDP([]byte("Hi"), conn.LocalAddr().(*net.UDPAddr))

	// This time Read should succeed.
	n, err := conn.Read(buf)
	if err != nil {
		return err
	}

	fmt.Printf("Read: %q\n", buf[:n])

	return nil
}
