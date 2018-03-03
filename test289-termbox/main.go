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

	go createUserActions()
	go createSystemActions()

	const (
		minX = 1
		maxX = 9
		minY = 1
		maxY = 4
	)
	const (
		edgeTop = iota
		edgeRight
		edgeBottom
		edgeLeft
	)
	x := minX
	y := minY
	edge := edgeTop

	termbox.SetCursor(x, y)
	termbox.Flush()

	for act := range actions {
		switch act.Type {
		case ActInputChar:
			dispatch(Action{
				Type: ActPutChar,
				Ch:   act.Ch,
			})
			dispatch(Action{
				Type: ActAdvanceCursor,
			})

		case ActPutChar:
			termbox.SetCell(x, y, act.Ch, termbox.ColorDefault, termbox.ColorDefault)
			termbox.Flush()

		case ActAdvanceCursor:
			switch edge {
			case edgeTop:    if x++; x == maxX { edge = edgeRight  }
			case edgeRight:  if y++; y == maxY { edge = edgeBottom }
			case edgeBottom: if x--; x == minX { edge = edgeLeft   }
			case edgeLeft:   if y--; y == minY { edge = edgeTop    }
			}
			termbox.SetCursor(x, y)
			termbox.Flush()

		case ActQuit:
			return act.Err
		}
	}

	return nil
}
