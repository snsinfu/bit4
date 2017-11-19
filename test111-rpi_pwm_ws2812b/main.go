package main

import (
	"fmt"
	"time"

	"periph.io/x/periph"
	"periph.io/x/periph/conn/gpio/gpiostream"
	"periph.io/x/periph/host/bcm283x"
)

func main() {
	if err := run(); err != nil {
		fmt.Println("error:", err)
	}
}

func run() error {
	if _, err := periph.Init(); err != nil {
		return err
	}

	pattern := make([]byte, 1000)
	for i := range pattern {
		if i%100 < 50 {
			pattern[i] = 255
		} else {
			pattern[i] = 0
		}
	}

	bits := gpiostream.BitStream{
		Bits: pattern,
		Res:  time.Millisecond,
	}

	pin := bcm283x.GPIO18
	if err := pin.StreamOut(&bits); err != nil {
		return err
	}

	return nil
}
