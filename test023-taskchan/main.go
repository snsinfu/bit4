package main

import (
	"errors"
	"fmt"

	"github.com/frickiericker/bit4/test023-taskchan/taskch"
)

func main() {
	tch := taskch.New()

	tch.Go(func() error {
		return errors.New("foo")
	})

	if err := tch.Wait(); err != nil {
		fmt.Println(err)
	}
}
