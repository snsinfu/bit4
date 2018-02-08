package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		var beg int
		var end int
		var value float64

		if _, err := fmt.Sscan(scanner.Text(), &beg, &end, &value); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			os.Exit(1)
		}

		fmt.Printf("[%d, %d): %g\n", beg, end, value)
	}
}
