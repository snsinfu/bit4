package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
)

func main() {
	const socket_path = "test.sock"

	// Need to unlink the path, if any, before Listen.
	if err := os.Remove(socket_path); err != nil {
		if !os.IsNotExist(err) {
			log.Fatal(err)
		}
	}

	// Datagram has difficulties. Stream is enough.
	listener, err := net.Listen("unix", socket_path)
	if err != nil {
		log.Fatal(err)
	}

	// TODO: trap signal
	defer listener.Close()

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal(err)
		}

		// We serve synchronously.
		defer conn.Close()

		if err := serve(conn); err != nil {
			log.Fatal(err)
		}
	}
}

func serve(conn net.Conn) error {
	buf := make([]byte, 100)

	// Long timeout for communicating with netcat.
	conn.SetDeadline(time.Now().Add(5 * time.Second))

	for {
		n, err := conn.Read(buf)
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		log.Println(n)
	}
}
