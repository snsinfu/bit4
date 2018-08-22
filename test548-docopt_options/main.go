package main

import (
	"fmt"
	"os"

	"github.com/docopt/docopt-go"
)

const usage = `
usage: main [-ph] <host>

options:
  -p, --port <port>  Specify port number [default: 80]
  -h, --help         Show this help message and exit
`

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	// Flag option after positional argument
	argv := []string{"example.com", "--port", "1234"}

	opts, err := docopt.ParseArgs(usage, argv, "")
	if err != nil {
		return err
	}

	port, err := opts.Int("--port")
	if err != nil {
		return err
	}

	host, err := opts.String("<host>")
	if err != nil {
		return err
	}

	fmt.Println("port:", port)
	fmt.Println("host:", host)

	return nil
}
