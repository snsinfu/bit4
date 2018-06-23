package wsock

import (
	"testing"

	"github.com/gorilla/websocket"
)

func Test_isClosedError_FlagsRealClosedSocketError(t *testing.T) {
	ws, _, err := websocket.DefaultDialer.Dial("ws://"+testingHostAddress+"/echo", nil)
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}
	ws.Close()

	_, _, err = ws.ReadMessage()
	if !isClosedError(err) {
		t.Errorf("closed error is expected, got: %s", err)
	}
}

func Test_isClosedError_DoesNotFlagNil(t *testing.T) {
	if isClosedError(nil) {
		t.Errorf("nil should not be an error")
	}
}
