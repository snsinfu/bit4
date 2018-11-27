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

	scr.SetStyle(
		tcell.StyleDefault.
			Foreground(tcell.ColorWhite).
			Background(tcell.ColorBlack),
	)
	scr.Clear()

	quit := make(chan bool)
	update := make(chan bool)

	go func() {
		for {
			ev := scr.PollEvent()

			switch ev := ev.(type) {
			case *tcell.EventKey:
				switch ev.Key() {
				case tcell.KeyEscape, tcell.KeyCtrlC:
					quit <- true
					return

				case tcell.KeyRune:
					switch ev.Rune() {
					case 'q', 'Q':
						quit <- true
						return
					}

				case tcell.KeyCtrlL:
					scr.Sync()
				}

			case *tcell.EventResize:
				update <- true
			}
		}
	}()

	timeout := time.After(20*time.Second)
	tick := time.Tick(time.Second)

	styleInfo := tcell.
		StyleDefault.
		Foreground(tcell.ColorWhite).
		Background(tcell.ColorBlack)

	styleStrip := tcell.
		StyleDefault.
		Foreground(tcell.ColorBlack).
		Background(tcell.ColorGreen)

	styleWarning := tcell.
		StyleDefault.
		Foreground(tcell.ColorYellow).
		Background(tcell.ColorBlack)

loop:
	for {
		w, h := scr.Size()

		scr.Clear()

		clock := time.Now().Format(time.Stamp)
		for i, c := range clock {
			scr.SetContent(w - len(clock) + i, 0, c, nil, styleInfo)
		}

		for x := 0; x < w; x++ {
			scr.SetContent(x, 1, rune("-o"[x % 2]), nil, styleStrip)
		}

		for y := 2; y < h; y++ {
			scr.SetContent(1, y, '/', nil, styleWarning)
			scr.SetContent(w - 2, y, '/', nil, styleWarning)
		}

		msg := "Press Q, Esc or Ctrl-C to exit"
		for i, c := range msg {
			scr.SetContent(i, 0, c, nil, styleInfo)
		}

		scr.Show()

		select {
		case <-quit:
			break loop

		case <-timeout:
			break loop

		case <-update:
			break

		case <-tick:
			break
		}
	}

	return nil
}
