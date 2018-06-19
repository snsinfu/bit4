package main

import (
	"log"

	"github.com/gorilla/websocket"
)

const (
	websocketURL = "ws://localhost:9000/"
	bufferSize   = 1460
)

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	session, err := Dial(websocketURL)
	if err != nil {
		return err
	}

	for msg := range session.R {
		session.W <- msg
	}

	return nil
}

type Session struct {
	R <-chan []byte
	W chan<- []byte
}

func Dial(url string) (*Session, error) {
	ws, _, err := websocket.DefaultDialer.Dial(url, nil)
	if err != nil {
		return nil, err
	}

	rch := make(chan []byte, 1)
	wch := make(chan []byte, 1)

	go func() error {
		buf := make([]byte, bufferSize)

		for {
			_, r, err := ws.NextReader()
			if err != nil {
				return err
			}

			n, err := r.Read(buf)
			if err != nil {
				return err
			}

			// BUG: buf can be overwritten by next read...
			rch <- buf[:n]
		}
	}()

	go func() error {
		for msg := range wch {
			if err := ws.WriteMessage(websocket.BinaryMessage, msg); err != nil {
				return err
			}
		}
		return nil
	}()

	return &Session{
		R: rch,
		W: wch,
	}, nil
}
