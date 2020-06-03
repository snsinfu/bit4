package main

import (
	"fmt"
	"time"
)

func main() {
	p := NewPeriodic(time.Second, func() {
		fmt.Println(time.Now())
	})
	time.Sleep(3 * time.Second)
	p.Stop()
	time.Sleep(3 * time.Second)
	fmt.Println("Done")
}

type Periodic struct {
	end chan bool
}

func NewPeriodic(d time.Duration, f func()) *Periodic {
	end := make(chan bool)
	go func() {
		tick := time.Tick(d)
		for {
			select {
			case <-tick:
				f()
			case <-end:
				close(end)
				return
			}
		}
	}()
	return &Periodic{end}
}

func (p *Periodic) Stop() {
	p.end <- true
	<-p.end
}
