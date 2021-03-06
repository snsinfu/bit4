package main

import (
	"fmt"
	"io"
	"os"
)

func main() {
	r, w := io.Pipe()

	go func() {
		for i := 0; i < 100; i++ {
			w.Write([]byte(fmt.Sprintf("%d", i)))
		}
		w.Close()
	}()

	ur := newUnreliableReader(r)
	buf := make([]byte, 1024)

	for {
		n, err := ur.Read(buf)
		if err != nil {
			if err == io.EOF {
				break
			}
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}

		fmt.Printf("@ %q\n", buf[:n])
	}
}

type unreliableReader struct {
	src     io.Reader
	delay   []byte
	counter int
}

func newUnreliableReader(r io.Reader) *unreliableReader {
	return &unreliableReader{
		src: r,
	}
}

func (ur *unreliableReader) Read(buf []byte) (int, error) {
	ur.counter++

	switch ur.counter % 10 {
	case 5, 9:
		// Successful read
		return ur.src.Read(buf)

	case 2, 4, 8:
		// Read from delay buffer
		if ur.delay != nil {
			n := len(ur.delay)
			copy(buf, ur.delay)
			ur.delay = nil
			return n, nil
		}

		ur.delay = make([]byte, 1024)
		n, err := ur.src.Read(ur.delay)
		if err != nil {
			return 0, err
		}
		ur.delay = ur.delay[:n]

		return ur.Read(buf)

	default:
		// Drop
		if _, err := ur.src.Read(buf); err != nil {
			return 0, err
		}
		return ur.Read(buf)
	}
}
