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
	w := uilive.New()
	w.Start()

	for i := 0; i < 100; i++ {

		x1 := i / 2
		x2 := 100 - x1

		// Need to re-render everything in every frame.
		fmt.Fprintf(w, "1: %s %d\n", spinner[x1%len(spinner)], x1)
		fmt.Fprintf(w, "2: %s %d\n", spinner[x2%len(spinner)], x2)
		w.Flush()

		time.Sleep(100 * time.Millisecond)
	}

	fmt.Fprintln(w, "1: OK")
	fmt.Fprintln(w, "2: OK")
	w.Flush()

	w.Stop()
}
