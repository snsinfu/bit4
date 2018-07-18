package main

import (
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test477-beacon/client"
)

func main() {
	if err := client.Start(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}
