package main

import (
	"os"
	"os/signal"
	"syscall"
)

func createSystemActions() {
	signals := make(chan os.Signal)

	signal.Notify(signals, syscall.SIGINT)
	signal.Notify(signals, syscall.SIGTERM)

	<-signals

	dispatch(Action{
		Type: ActQuit,
	})
}
