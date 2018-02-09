package main

import (
	"fmt"
	"io"
	"os"

	"github.com/docopt/docopt-go"
	"github.com/snsinfu/bit4/test257-clio/clio"
)

const usage = `
Concatenate inputs.

Usage:
  cat [-o FILE] [<input> ...]
  cat -h

Options:
  -o FILE, --output FILE  Specify output file [default: -]
  -h, --help              Show this help message and exit
`

type command struct {
	Input  []string
	Output string
	Help   bool
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	docopts, err := docopt.ParseDoc(usage)
	if err != nil {
		return err
	}

	cmd := command{}
	if err := docopts.Bind(&cmd); err != nil {
		return err
	}

	return cmd.run()
}

func (cmd *command) run() error {
	output, err := clio.Create(cmd.Output)
	if err != nil {
		return err
	}
	defer output.Close()

	if len(cmd.Input) == 0 {
		_, err := io.Copy(output, os.Stdin)
		return err
	}

	for _, name := range cmd.Input {
		input, err := clio.Open(name)
		if err != nil {
			return err
		}
		defer input.Close()

		if _, err := io.Copy(output, os.Stdin); err != nil {
			return err
		}
	}
	return nil
}
