package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	s := encodeUint64(1234567890)
	_, v := decodeUint64(s)

	fmt.Println(s)
	fmt.Println(v)

	return nil
}

// The DIS (Data Is Strings) format used in PBS communication protocol.

func encodeUint64(val uint64) string {
	repr := strconv.FormatUint(val, 10)
	count := uint64(len(repr))
	repr = "+" + repr

	for count > 1 {
		s := strconv.FormatUint(count, 10)
		count = uint64(len(s))
		repr = s + repr
	}

	return repr
}

func decodeUint64(s string) (string, uint64) {
	count := 1

	for {
		if s[0] == '+' {
			s = s[1:]
			break
		}

		n, _ := strconv.ParseUint(s[:count], 10, 64)
		s = s[count:]
		count = int(n)
	}

	val, _ := strconv.ParseUint(s[:count], 10, 64)
	s = s[count:]

	return s, val
}
