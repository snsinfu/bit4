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

	app := New(new(Domain), scr)

	sig := make(chan os.Signal)
	signal.Notify(sig, syscall.SIGINT)
	signal.Notify(sig, syscall.SIGTERM)

	go func() {
		for range sig {
			app.Quit()
		}
	}()

	// First launch
	app.Start()

	scr.Clear()
	for i, c := range "Restarting!" {
		scr.SetContent(3+i, 3, c, nil, 0)
	}
	scr.Sync()
	time.Sleep(time.Second)

	// App can restart
	return app.Start()
}

// Domain is the domain logic behind the application. Make this interface if you
// want to mock domain logic when testing app.
type Domain struct {
}

func (dom *Domain) Query() string {
	return time.Now().Format(time.Stamp)
}

// App is a TUI application.
type App struct {
	dom    *Domain
	scr    tcell.Screen
	quit   chan bool
	update chan bool
	tick   time.Duration
}

// New creates an app on given screen.
func New(dom *Domain, scr tcell.Screen) *App {
	return &App{
		dom:    dom,
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
		if ev == nil {
			break
		}

		switch ev := ev.(type) {
		case *tcell.EventKey:
			switch ev.Key() {
			case tcell.KeyCtrlC:
				app.quit <- true

			case tcell.KeyRune:
				switch ev.Rune() {
				case 'q', 'Q':
					app.quit <- true
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

	info := app.dom.Query()
	for i, c := range info {
		scr.SetContent(w-len(info)+i, 0, c, nil, styleInfo)
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
