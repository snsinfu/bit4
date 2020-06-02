package main

import (
	"fmt"
	"time"

	"github.com/gosuri/uilive"
)

var spinner = []string{
	"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏",
}

func main() {
	w1 := uilive.New()
	w2 := uilive.New()

	w1.Start()
	w2.Start()

	// No, this does not work.

	for i := 0; i < 100; i++ {
		fmt.Fprintf(w1, "1: %s %d\n", spinner[i%len(spinner)], i)
		fmt.Fprintf(w2, "2: %s %d\n", spinner[i%len(spinner)], i)

		time.Sleep(100 * time.Millisecond)
	}

	fmt.Fprintln(w1, "1: OK")
	fmt.Fprintln(w2, "2: OK")

	w1.Stop()
	w2.Stop()
}
