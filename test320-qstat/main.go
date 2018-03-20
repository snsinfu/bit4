package main

import (
	"fmt"
	"io/ioutil"
	"os"

	"./qstat"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	src, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		return err
	}

	jobs, err := qstat.ParseJobs(string(src))
	if err != nil {
		return err
	}

	for id, job := range jobs {
		fmt.Println(id, ":", job)
	}
	return nil
}
