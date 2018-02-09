// Apply 1-to-N mappings over input sequence. Go does not have pythonic
// generators, so we need to use a callback or a channel [1].
//
// [1]: https://stackoverflow.com/questions/34464146

package main

import (
	"fmt"
	"io"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
	}
}

func run() error {
	g := HelloGenerator{func(msg string) error {
		_, err := fmt.Println(msg)
		return err
	}}

	for {
		var n int

		if _, err := fmt.Scan(&n); err != nil {
			if err == io.EOF {
				return nil
			}
			return err
		}

		g.Feed(n)
	}

	return nil
}

type HelloGenerator struct {
	yield func(string) error
}

func (g *HelloGenerator) Feed(n int) error {
	for i := 0; i < n; i++ {
		if err := g.yield("hello"); err != nil {
			return err
		}
	}
	return nil
}
