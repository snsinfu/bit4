package session

import (
	"testing"

	"github.com/gorilla/websocket"
)

func Test_reinterpretError_TranslatesClosedError(t *testing.T) {
	ws, _, err := websocket.DefaultDialer.Dial("ws://"+testingHostAddress+"/echo", nil)
	if err != nil {
		t.Fatalf("unexpected Dial error: %s", err)
	}
	ws.Close()

	_, _, err = ws.ReadMessage()
	err = reinterpretError(err)
	if err != ErrClosed {
		t.Fatalf("closed error is expected, got: %s", err)
	}
}
