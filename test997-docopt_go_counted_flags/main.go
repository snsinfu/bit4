package main

import (
	"fmt"
	"os"

	"github.com/docopt/docopt-go"
)

/*
// This does not work!
const usage = `
usage: main [options]

options:
  -v    verbose
  -vv   more verbose
`
*/

/*
// This works but ugly
const usage = `
usage: main [-v | -vv]
`
*/

// Same.
const usage = `
usage: main [-v ...]
`


func main() {
	parser := docopt.Parser{
		HelpHandler:  docopt.PrintHelpAndExit,
		OptionsFirst: true,
	}
	opts, err := parser.ParseArgs(usage, nil, "")
	if err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}

	fmt.Println(opts)
}
