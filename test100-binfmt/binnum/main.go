package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
)

func main() {
	if err := run(os.Stdin, os.Stdout); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(src io.Reader, dst io.Writer) error {
	scanner := bufio.NewScanner(src)
	for scanner.Scan() {
		binRepr, err := ReformatAsBinary(scanner.Text())
		if err != nil {
			return err
		}
		if _, err := fmt.Fprintln(dst, binRepr); err != nil {
			return err
		}
	}
	return scanner.Err()
}

func ReformatAsBinary(repr string) (string, error) {
	number, err := strconv.ParseInt(repr, 10, 64)
	if err != nil {
		return "", err
	}
	return strconv.FormatInt(number, 2), nil
}
