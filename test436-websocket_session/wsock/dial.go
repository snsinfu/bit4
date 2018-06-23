package wsock

import (
	"time"

	"github.com/gorilla/websocket"
)

var (
	dialer = websocket.DefaultDialer
)

// Dial connects to a websocket endpoint and returns a session object. The
// session is closed after specified duration of no communication.
func Dial(url string, timeout time.Duration) (*Session, error) {
	ws, _, err := dialer.Dial(url, nil)
	if err != nil {
		return nil, err
	}

	s := &Session{
		ws:      ws,
		timeout: 0,
	}

	go s.checkTimeout(timeout)

	return s, nil
}
