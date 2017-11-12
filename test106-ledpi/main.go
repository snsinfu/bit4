package main

import (
	"time"

	"periph.io/x/periph/conn/gpio"
	"periph.io/x/periph/host"
	"periph.io/x/periph/host/bcm283x"
)

func main() {
	host.Init()
	pin := bcm283x.GPIO18
	for {
		pin.Out(gpio.Low)
		time.Sleep(500 * time.Millisecond)
		pin.Out(gpio.High)
		time.Sleep(time.Second)
	}
}
