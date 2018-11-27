package main

import (
	"fmt"
	"os"
	"time"

	"github.com/gdamore/tcell"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
	time.Sleep(time.Second)
}

func run() error {
	scr, err := tcell.NewScreen()
	if err != nil {
		return err
	}

	if err := scr.Init(); err != nil {
		return err
	}
	defer scr.Fini()

	go func() {
		for range time.Tick(100*time.Millisecond) {
			ev := scr.PollEvent()
			fmt.Println("event:", ev)
			if ev == nil {
				break
			}
		}
	}()

	time.Sleep(time.Second)

	return nil
}
