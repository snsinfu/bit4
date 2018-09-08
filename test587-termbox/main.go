package main

import (
	"fmt"
	"os"

	"github.com/nsf/termbox-go"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	if err := termbox.Init(); err != nil {
		return err
	}
	defer termbox.Close()

	termbox.Clear(termbox.ColorDefault, termbox.ColorDefault)

	bars := []struct {
		width int
		color termbox.Attribute
	}{
		{10, termbox.ColorRed},
		{15, termbox.ColorGreen},
		{8, termbox.ColorYellow},
		{5, termbox.ColorBlue},
		{12, termbox.ColorMagenta},
		{16, termbox.ColorCyan},
	}

	for j, bar := range bars {
		for i := 0; i < bar.width; i++ {
			termbox.SetCell(5+i, 5+j, ' ', termbox.ColorDefault, bar.color)
		}
	}

	termbox.Flush()

keyLoop:
	for {
		switch ev := termbox.PollEvent(); ev.Type {
		case termbox.EventKey:
			if ev.Ch == 0 {
				if ev.Key == termbox.KeyCtrlC {
					break keyLoop
				}
			}

		case termbox.EventError:
			return ev.Err
		}
	}

	return nil
}
