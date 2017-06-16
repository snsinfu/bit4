package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func run() error {

	binSize := 15
	winSize := 20
	unitSize := gcd(binSize, winSize)
	fmt.Println(unitSize)

	scanner := bufio.NewScanner(os.Stdin)
	delimiter := "\t"

	// Data
	for scanner.Scan() {
		record := strings.Split(scanner.Text(), delimiter)

		_, err1 := strconv.ParseInt(record[0], 10, 64)
		_, err2 := strconv.ParseInt(record[1], 10, 64)
		_, err3 := strconv.ParseFloat(record[2], 64)

		if err := firstErr(err1, err2, err3); err != nil {
			return err
		}
	}

	return scanner.Err()
}

func firstErr(errs ...error) error {
	for _, err := range errs {
		if err != nil {
			return err
		}
	}
	return nil
}

func gcd(n, m int) int {
	for n > 0 {
		m, n = n, m%n
	}
	return m
}
