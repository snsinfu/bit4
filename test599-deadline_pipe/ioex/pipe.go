package ioex

import (
	"errors"
	"time"
)

var ErrTimeout = errors.New("timeout")


type Pipe struct {
	data     chan []byte
	ack      chan int
	rTimeout *time.Timer
	wTimeout *time.Timer
}

func NewPipe() *Pipe {
	return &Pipe{
		data:     make(chan []byte),
		ack:      make(chan int),
		rTimeout: cancel(time.NewTimer(0)),
		wTimeout: cancel(time.NewTimer(0)),
	}
}

func cancel(timer *time.Timer) *time.Timer {
	if !timer.Stop() {
		<-timer.C
	}
	return timer
}

// SetReadDeadline sets the deadline for a Read call.
func (p *Pipe) SetReadDeadline(t time.Time) error {
	if t.IsZero() {
		cancel(p.rTimeout)
	} else {
		p.rTimeout.Reset(time.Until(t))
	}
	return nil
}

// SetWriteDeadline sets the deadline for a Write call.
func (p *Pipe) SetWriteDeadline(t time.Time) error {
	if t.IsZero() {
		cancel(p.wTimeout)
	} else {
		p.wTimeout.Reset(time.Until(t))
	}
	return nil
}

func (p *Pipe) Read(buf []byte) (int, error) {
	// FIXME: Need lock
	select {
	case data := <-p.data:
		n := copy(buf, data)
		p.ack <- n
		return n, nil

	case <-p.rTimeout.C:
		return 0, ErrTimeout
	}
}

func (p *Pipe) Write(buf []byte) (int, error) {
	// FIXME: Need lock
	select {
	case p.data <- buf:
		return <-p.ack, nil

	case <-p.wTimeout.C:
		return 0, ErrTimeout
	}
}
