package main

import (
	"errors"
	"fmt"
	"net"
	"os"
	"strings"
	"time"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	conn, err := net.Dial("tcp", "localhost:4000")
	if err != nil {
		return err
	}
	defer conn.Close()

	timeout := time.After(5 * time.Second)
	go func() {
		<-timeout
		conn.Close()
	}()

	buf := make([]byte, 1)
	if _, err := conn.Read(buf); err != nil {
		// https://github.com/golang/go/issues/4373
		if strings.HasSuffix(err.Error(), "use of closed network connection") {
			return errors.New("read canceled")
		}
		return err
	}

	return nil
}
