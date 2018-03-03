package main

import (
	"github.com/nsf/termbox-go"
)

func createUserActions() {
	for {
		switch ev := termbox.PollEvent(); ev.Type {
		case termbox.EventKey:
			if ev.Ch == 0 {
				handleSpecialKey(ev.Key)
			} else {
				handleLetterKey(ev.Ch)
			}

		case termbox.EventError:
			dispatch(Action{
				Type: ActQuit,
				Err:  ev.Err,
			})
		}
	}
}

func handleSpecialKey(key termbox.Key) {
	switch key {
	case termbox.KeyCtrlC, termbox.KeyCtrlD:
		dispatch(Action{
			Type: ActQuit,
		})

	case termbox.KeySpace:
		dispatch(Action{
			Type: ActInputChar,
			Ch:   ' ',
		})
	}
}

func handleLetterKey(ch rune) {
	dispatch(Action{
		Type: ActInputChar,
		Ch:   ch,
	})
}
