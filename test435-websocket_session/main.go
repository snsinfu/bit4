package main

import (
	"log"
	"strings"
	"sync/atomic"
	"time"

	"github.com/gorilla/websocket"
)

const (
	websocketURL   = "ws://localhost:9000"
	bufferSize     = 1458
	sessionTimeout = 10 * time.Second
	messageTimeout = 3 * time.Second
)

// $ wscat --listen 9000
// $ go run main.go

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	s, err := Dial(websocketURL)
	if err != nil {
		return err
	}
	defer s.Close()

	buf := make([]byte, bufferSize)

	for {
		n, err := s.Read(buf)
		if err != nil {
			if isClosedError(err) {
				break
			}
			return err
		}

		if err := s.Write(buf[:n]); err != nil {
			return err
		}
	}

	log.Print("Timeout")

	return nil
}

type Session struct {
	ws      *websocket.Conn
	timeout int32
}

// Dial connects to a websocket endpoint.
func Dial(url string) (*Session, error) {
	ws, _, err := websocket.DefaultDialer.Dial(url, nil)
	if err != nil {
		return nil, err
	}

	s := &Session{
		ws:      ws,
		timeout: 0,
	}

	go func() {
		ticker := time.NewTicker(sessionTimeout / 2)
		defer ticker.Stop()

		for _ = range ticker.C {
			if atomic.AddInt32(&s.timeout, 1) == 2 {
				break
			}
		}

		ws.WriteControl(
			websocket.CloseMessage,
			websocket.FormatCloseMessage(websocket.CloseGoingAway, ""),
			time.Now().Add(messageTimeout),
		)
		ws.Close()
	}()

	return s, nil
}

// Close forcefully closes the websocket.
func (s *Session) Close() error {
	return s.ws.Close()
}

// Read reads a binary message from the websocket. Remaining bytes are discarded
// if buffer is smaller than the message. Timeout is canceled.
func (s *Session) Read(buf []byte) (int, error) {
	_, r, err := s.ws.NextReader()
	if err != nil {
		return 0, err
	}

	n, err := r.Read(buf)
	if err != nil {
		return n, err
	}

	atomic.StoreInt32(&s.timeout, 0)

	return n, err
}

// Write writes a binary message to the websocket. Timeout is canceled.
func (s *Session) Write(msg []byte) error {
	err := s.ws.WriteMessage(websocket.BinaryMessage, msg)
	if err != nil {
		return err
	}

	atomic.StoreInt32(&s.timeout, 0)

	return nil
}

// isClosedError checks if the given error is caused by a use of a closed
// network connection.
func isClosedError(err error) bool {
	// See: https://github.com/golang/go/issues/4373
	return strings.Contains(err.Error(), "use of closed network connection")
}
