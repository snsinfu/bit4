package main

import (
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test477-beacon/server"
)

func main() {
	if err := server.Start(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}
