package main

import (
	"fmt"
	"os"
	"time"

	"github.com/gorilla/websocket"

	"./session"
)

const (
	websocketURL   = "ws://localhost:8080"
	sessionTimeout = 10 * time.Second
	bufferSize     = 1458
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	ws, _, err := websocket.DefaultDialer.Dial(websocketURL, nil)
	if err != nil {
		return err
	}

	s := session.Wrap(ws, sessionTimeout)
	buf := make([]byte, bufferSize)

	for {
		n, err := s.Read(buf)
		if err == session.ErrClosed {
			break
		}
		if err != nil {
			return err
		}

		fmt.Println(buf[:n])
	}

	return nil
}
