package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"os/signal"
)

func main() {
	const socket_path = "test.sock"

	// Datagram has difficulties. Stream is enough.
	listener, err := net.Listen("unix", socket_path)
	if err != nil {
		log.Fatal(err)
	}
	defer os.Remove(socket_path)
	defer listener.Close()

	signals := make(chan os.Signal, 1)
	done := false
	go func() {
		<-signals
		listener.Close()
		done = true
	}()
	signal.Notify(signals, os.Interrupt)

	for !done {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}

		// We serve synchronously.
		defer conn.Close()

		if err := serve(conn); err != nil {
			log.Print(err)
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
