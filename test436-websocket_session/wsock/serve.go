package wsock

import (
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

var (
	upgrader = websocket.Upgrader{}
)

// Serve upgrades an HTTP session to a websocket session. The session is closed
// after specified duration of no communication.
func Serve(w http.ResponseWriter, r *http.Request, timeout time.Duration) (*Session, error) {
	ws, err := upgrader.Upgrade(w, r, nil)
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
