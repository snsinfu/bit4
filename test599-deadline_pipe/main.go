package main

import (
	"fmt"
	"os"
	"time"

	"github.com/snsinfu/bit4/test599-deadline_pipe/ioex"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	p := ioex.NewPipe()

	go p.Write([]byte("The quick brown fox jumps over the lazy dog"))

	p.SetReadDeadline(time.Now().Add(time.Second))

	buf := make([]byte, 19)
	n, err := p.Read(buf)
	fmt.Printf("data: %q\n", buf[:n])
	fmt.Printf("err: %v\n", err)

	return nil
}
