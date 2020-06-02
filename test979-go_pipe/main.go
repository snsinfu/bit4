package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
)

func main() {
	wc := exec.Command("wc", "-l")

	stdin, err := wc.StdinPipe()
	if err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}

	stdout, err := wc.StdoutPipe()
	if err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}

	if err := wc.Start(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}

	fmt.Fprintln(stdin, "The quick brown fox")
	fmt.Fprintln(stdin, "jumps over")
	fmt.Fprintln(stdin, "the lazy dog")
	stdin.Close()

	result, err := ioutil.ReadAll(stdout)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}

	if err := wc.Wait(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}

	// Three lines, so the output should be "3" followed by a newline.
	fmt.Println(string(result) == "3\n")
}
