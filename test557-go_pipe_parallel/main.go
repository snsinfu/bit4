package main

// Writing to a pipe blocks until the content is read on the other end. So it
// is safe to pass same buffer in a loop and let goroutines read from pipes.

import (
	"fmt"
	"io"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	type session struct {
		w io.Writer
		c byte
	}
	sessions := []*session{}

	for i := 0; i < 16; i++ {
		r, w := io.Pipe()
		c := byte(i)
		go reader(r, c)
		sessions = append(sessions, &session{w: w, c: c})
	}

	buf := make([]byte, 32)
	i := 0

	for {
		sess := sessions[i % len(sessions)]
		i++

		sess.c++
		buf[0] = sess.c
		sess.w.Write(buf[:1])

		// If the pipe just sends buf via a channel and copies the content to a
		// read buffer on the other end, the content can get corrupted while
		// copying. The doc of io.Pipe clearly states that this won't happen,
		// and the implementation uses two channels to synchronize write and
		// read.
	}
}

func reader(r io.Reader, c byte) error {
	buf := make([]byte, 32)
	prev := c

	for {
		_, err := r.Read(buf)
		if err != nil {
			return err
		}

		delta := buf[0] - prev
		if delta != 1 {
			fmt.Println("!!")
		}
		prev = buf[0]
	}
}
