package session

import (
	"reflect"
	"testing"
	"time"

	"github.com/gorilla/websocket"
)

func Test_Session_CommunicatesWithServer(t *testing.T) {
	ws, _, err := websocket.DefaultDialer.Dial("ws://"+testingHostAddress+"/echo", nil)
	if err != nil {
		t.Fatalf("unexpected Dial error: %s", err)
	}

	s := Wrap(ws, 60*time.Second)

	msg := []byte{2, 3, 5, 7, 11}
	if err := s.Write(msg); err != nil {
		t.Fatalf("unexpected Write error: %s", err)
	}

	buf := make([]byte, 10)
	n, err := s.Read(buf)
	if err != nil {
		t.Fatalf("unexpected Read error: %s", err)
	}

	if !reflect.DeepEqual(msg, buf[:n]) {
		t.Fatalf("unexpected message: actual %v, expected %v", buf[:n], msg)
	}

	if err := s.Close(); err != nil {
		t.Fatalf("unexpected Close error: %s", err)
	}
}

func Test_Session_ClosesOnReadTimeout(t *testing.T) {
	ws, _, err := websocket.DefaultDialer.Dial("ws://"+testingHostAddress+"/ticker", nil)
	if err != nil {
		t.Fatalf("unexpected Dial error: %s", err)
	}

	s := Wrap(ws, time.Millisecond)

	_, err = s.Read(make([]byte, 1))
	if err != ErrClosed {
		t.Fatalf("expected timeout error from Read, got: %s", err)
	}

	err = s.Close()
	if err != ErrClosed {
		t.Fatalf("expected timeout error from Close, got: %s", err)
	}
}

func Test_Session_ClosesOnIdle(t *testing.T) {
	ws, _, err := websocket.DefaultDialer.Dial("ws://"+testingHostAddress+"/ticker", nil)
	if err != nil {
		t.Fatalf("unexpected Dial error: %s", err)
	}

	s := Wrap(ws, time.Millisecond)

	// Long idle session...
	time.Sleep(50 * time.Millisecond)

	err = s.Write([]byte{1})
	if err != websocket.ErrCloseSent {
		t.Fatalf("expected CloseSent error from Write, got: %s", err)
	}

	err = s.Close()
	if err != ErrClosed {
		t.Fatalf("expected Closed error from Close, got: %s", err)
	}
}
