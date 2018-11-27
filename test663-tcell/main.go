package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
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

	app := New(scr)

	sig := make(chan os.Signal)
	signal.Notify(sig, syscall.SIGINT)
	signal.Notify(sig, syscall.SIGTERM)

	go func() {
		<-sig
		app.Quit()
	}()

	return app.Start()
}

// App is a TUI application.
type App struct {
	scr    tcell.Screen
	quit   chan bool
	update chan bool
	tick   time.Duration
}

// New creates an app on given screen.
func New(scr tcell.Screen) *App {
	return &App{
		scr:    scr,
		quit:   make(chan bool),
		update: make(chan bool),
		tick:   time.Second,
	}
}

// Start starts an app.
func (app *App) Start() error {
	scr := app.scr
	scr.Clear()

	go app.dispatcher()

	tick := time.Tick(app.tick)

loop:
	for {
		app.draw()

		select {
		case <-app.quit:
			break loop

		case <-app.update:
			scr.Clear()
			break

		case <-tick:
			break
		}
	}

	return nil
}

// Quit commands app to quit.
func (app *App) Quit() {
	app.quit <- true
}

// dispatcher polls terminal events and dispatches app events.
func (app *App) dispatcher() {
	for {
		ev := app.scr.PollEvent()

		switch ev := ev.(type) {
		case *tcell.EventKey:
			switch ev.Key() {
			case tcell.KeyCtrlC:
				app.quit <- true
				return

			case tcell.KeyRune:
				switch ev.Rune() {
				case 'q', 'Q':
					app.quit <- true
					return
				}

			case tcell.KeyCtrlL:
				app.scr.Sync()
			}

		case *tcell.EventResize:
			app.update <- true
		}
	}
}

// draw draws screen.
func (app *App) draw() {
	scr := app.scr

	styleInfo := tcell.StyleDefault

	styleStrip := tcell.
		StyleDefault.
		Foreground(tcell.ColorBlack).
		Background(tcell.ColorGreen)

	styleWarning := tcell.
		StyleDefault.
		Foreground(tcell.ColorYellow)

	w, h := scr.Size()

	msg := "Press Q or Ctrl-C to exit"
	for i, c := range msg {
		scr.SetContent(i, 0, c, nil, styleInfo)
	}

	clock := time.Now().Format(time.Stamp)
	for i, c := range clock {
		scr.SetContent(w-len(clock)+i, 0, c, nil, styleInfo)
	}

	for x := 0; x < w; x++ {
		scr.SetContent(x, 1, rune("-o"[x%2]), nil, styleStrip)
	}

	for y := 2; y < h; y++ {
		scr.SetContent(1, y, '/', nil, styleWarning)
		scr.SetContent(w-2, y, '/', nil, styleWarning)
	}

	scr.Show()
}
