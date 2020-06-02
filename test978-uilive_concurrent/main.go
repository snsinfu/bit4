package main

import (
	"fmt"
	"sync"
	"time"

	"github.com/gosuri/uilive"
)

const (
	escReset = "\x1b[0m"
	escGreen = "\x1b[32m"
	escGray  = "\x1b[90m"
)

var spinner = []string{
	"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏",
}

type update struct {
	Row  int
	Text string
}

func task(row int, sleep time.Duration, updates chan<- update) {
	for i := 0; i < 100; i++ {
		updates <- update{
			Row:  row,
			Text: fmt.Sprintf("%d: %s %d", row, spinner[i%len(spinner)], i),
		}
		time.Sleep(sleep)
	}
	updates <- update{
		Row:  row,
		Text: fmt.Sprintf("%d: %sOK%s", row, escGreen, escReset),
	}
}

func main() {
	n := 10

	updates := make(chan update, 20)
	sem := make(chan int, 2)
	wg := sync.WaitGroup{}
	wg.Add(n)

	for i := 0; i < n; i++ {
		row := i
		go func() {
			updates <- update{
				Row:  row,
				Text: fmt.Sprintf("%d: %sWAIT%s", row, escGray, escReset),
			}

			sem <- 1
			sleepSec := 20 * (5*row%3 + 1)
			sleep := time.Duration(sleepSec) * time.Millisecond
			task(row, sleep, updates)
			<-sem

			wg.Done()
		}()
	}

	end := make(chan int)
	endOK := make(chan int)

	go func() {
		w := uilive.New()
		w.RefreshInterval = 100*time.Second
		w.Start()

		rows := make([]string, n)

	rendering:
		for range time.Tick(103 * time.Millisecond) {
		pump:
			for {
				select {
				case up := <-updates:
					rows[up.Row] = up.Text

				case <-end:
					break rendering

				default:
					break pump
				}
			}

			for _, row := range rows {
				fmt.Fprintln(w, row)
			}
			w.Flush()
		}

		w.Flush()
		w.Stop()
		endOK <- 1
	}()

	wg.Wait()
	end <- 1
	<-endOK
}
