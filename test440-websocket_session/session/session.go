package session

import (
	"sync/atomic"
	"time"

	"github.com/gorilla/websocket"
)

const (
	timeoutMessage        = "timeout"
	timeoutMessageTimeout = 3 * time.Second
)

// Session wraps a websocket connection with timeout state.
type Session struct {
	ws   *websocket.Conn
	idle int32
}

// Wrap wraps existing websocket connection as a Session with timeout.
func Wrap(ws *websocket.Conn, timeout time.Duration) *Session {
	s := &Session{
		ws:   ws,
		idle: 0,
	}

	go func() {
		ticker := time.NewTicker(timeout / 2)
		defer ticker.Stop()

		for range ticker.C {
			if atomic.AddInt32(&s.idle, 1) == 2 {
				break
			}
		}

		ws.WriteControl(
			websocket.CloseMessage,
			websocket.FormatCloseMessage(websocket.CloseGoingAway, timeoutMessage),
			time.Now().Add(timeoutMessageTimeout),
		)
		ws.Close()
	}()

	return s
}

// Close forcefully closes the websocket.
func (s *Session) Close() error {
	return reinterpretError(s.ws.Close())
}

// Read reads a binary message from the websocket. Remaining bytes are discarded
// if buffer is smaller than the message. Internal timeout timer is reset after
// successful read.
func (s *Session) Read(buf []byte) (int, error) {
	_, r, err := s.ws.NextReader()
	if err != nil {
		return 0, reinterpretError(err)
	}

	n, err := r.Read(buf)
	if err != nil {
		return n, err
	}

	atomic.StoreInt32(&s.idle, 0)

	return n, err
}

// Write writes a binary message to the websocket. Internal timeout timer is
// reset after successful write.
func (s *Session) Write(msg []byte) error {
	err := s.ws.WriteMessage(websocket.BinaryMessage, msg)
	if err != nil {
		return reinterpretError(err)
	}

	atomic.StoreInt32(&s.idle, 0)

	return nil
}
