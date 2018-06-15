// UDP Conn keeps its private port until it is closed, though UDP is connection
// less. Just it's reusing socket I guess.

package main

import (
	"fmt"
	"net"
	"os"
	"time"

	"../test023-taskchan/taskch"
)

const (
	serverAddress = "localhost:4000"
	bufferSize    = 1472
	messageCount  = 3
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	tasks := taskch.New()

	tasks.Go(server)
	tasks.Go(client)
	tasks.Go(client)

	return tasks.Wait()
}

func server() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

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

		fmt.Printf("%s -> '%s'\n", peer, string(buf[:n]))
	}
}

func client() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	port := conn.LocalAddr().(*net.UDPAddr).Port
	msg := []byte(fmt.Sprintf("Lorem ipsum my port is %d", port))

	ticker := time.NewTicker(time.Second)

	for i := 0; i < messageCount; i++ {
		<-ticker.C

		if _, err := conn.Write(msg); err != nil {
			return err
		}
	}

	return nil
}
