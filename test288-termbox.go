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
	termbox.Flush()

	const (
		edgeTop = iota
		edgeRight
		edgeBottom
		edgeLeft
	)

	const (
		minX = 1
		maxX = 6
		minY = 1
		maxY = 4
	)

	x := minX
	y := minY
	edge := edgeTop

	for {
		switch ev := termbox.PollEvent(); ev.Type {
		case termbox.EventKey:
			if ev.Ch == 0 {
				switch ev.Key {
				case termbox.KeyCtrlC:
					return nil
				}
			} else {
				termbox.SetCell(x, y, ev.Ch, termbox.ColorDefault, termbox.ColorDefault)
				termbox.Flush()

				switch edge {
				case edgeTop:    if x++; x == maxX { edge = edgeRight }
				case edgeRight:  if y++; y == maxY { edge = edgeBottom }
				case edgeBottom: if x--; x == minX { edge = edgeLeft }
				case edgeLeft:   if y--; y == minY { edge = edgeTop }
				}
			}

		case termbox.EventError:
			return ev.Err
		}
	}
}
