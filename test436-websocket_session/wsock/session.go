package wsock

import (
	"sync/atomic"
	"time"

	"github.com/gorilla/websocket"
)

// Session represents a websocket connection with timeout.
type Session struct {
	ws      *websocket.Conn
	timeout int32
}

// checkTimeout waits for timeout and closes the session if it's open.
func (s *Session) checkTimeout(timeout time.Duration) {
	ticker := time.NewTicker(timeout / 2)
	defer ticker.Stop()

	for _ = range ticker.C {
		if atomic.AddInt32(&s.timeout, 1) == 2 {
			break
		}
	}

	s.ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseGoingAway, "timeout"),
		time.Now().Add(3 * time.Second),
	)
	s.ws.Close()
}

// Close forcefully closes the websocket.
func (s *Session) Close() error {
	err := s.ws.Close()
	if isClosedError(err) {
		err = ErrTimeout
	}
	return err
}

// Read reads a binary message from the websocket. Remaining bytes are discarded
// if buffer is smaller than the message.
func (s *Session) Read(buf []byte) (int, error) {
	_, r, err := s.ws.NextReader()
	if err != nil {
		if isClosedError(err) {
			err = ErrTimeout
		}
		return 0, err
	}

	n, err := r.Read(buf)
	if err != nil {
		return n, err
	}

	atomic.StoreInt32(&s.timeout, 0)

	return n, err
}

// Write writes a binary message to the websocket.
func (s *Session) Write(msg []byte) error {
	err := s.ws.WriteMessage(websocket.BinaryMessage, msg)
	if err != nil {
		if isClosedError(err) {
			err = ErrTimeout
		}
		return err
	}

	atomic.StoreInt32(&s.timeout, 0)

	return nil
}
