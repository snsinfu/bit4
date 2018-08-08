package main

import (
	"io"
)

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
	default:
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

	case 0, 1, 3, 6, 7:
		// Drop
		if _, err := ur.src.Read(buf); err != nil {
			return 0, err
		}
		return ur.Read(buf)
	}
}
