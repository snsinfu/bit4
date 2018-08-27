package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
)

const (
	bufferSize  = 512
	delay       = 100*time.Millisecond
	srcAddress  = "127.0.0.1:9999"
	destAddress = "127.0.0.1:22"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	ln, err := net.Listen("tcp", srcAddress)
	if err != nil {
		return err
	}

	for {
		conn, err := ln.Accept()
		if err != nil {
			return err
		}

		go func() {
			defer conn.Close()
			forward(conn)
		}()
	}
}

func forward(src net.Conn) {
	log.Printf("[%s] session start", src.RemoteAddr())

	dest, err := net.Dial("tcp", destAddress)
	if err != nil {
		log.Printf("[%s] error: %s", src.RemoteAddr(), err)
		return
	}
	defer dest.Close()

	join := make(chan error)

	go func() {
		buf := make([]byte, bufferSize)

		for {
			n, err := src.Read(buf)
			if err != nil {
				join <- err
				return
			}

			time.Sleep(delay)

			if _, err := dest.Write(buf[:n]); err != nil {
				join <- err
				return
			}
		}
	}()

	go func() {
		buf := make([]byte, bufferSize)

		for {
			n, err := dest.Read(buf)
			if err != nil {
				join <- err
				return
			}

			time.Sleep(delay)

			if _, err := src.Write(buf[:n]); err != nil {
				join <- err
				return
			}
		}
	}()

	for i := 0; i < 2; i++ {
		if err := <-join; err != nil {
			log.Printf("[%s] error: %s", src.RemoteAddr(), err)
		}
	}
}
