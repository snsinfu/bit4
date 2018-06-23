package wsock

import (
	"reflect"
	"testing"
	"time"
)

func Test_Session_ClientCommunicatesWithServer(t *testing.T) {
	s, err := Dial("ws://"+testingHostAddress+"/echo", 60*time.Second)
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	msg := []byte{2, 3, 5, 7, 11}
	if err := s.Write(msg); err != nil {
		t.Errorf("unexpected Write error: %s", err)
	}

	buf := make([]byte, 10)
	n, err := s.Read(buf)
	if err != nil {
		t.Errorf("unexpected Read error: %s", err)
	}

	if !reflect.DeepEqual(msg, buf[:n]) {
		t.Errorf("unexpected message: actual %v, expected %v", buf[:n], msg)
	}

	if err := s.Close(); err != nil {
		t.Errorf("unexpected Close error: %s", err)
	}
}

func Test_Session_ClientClosesOnTimeout(t *testing.T) {
	s, err := Dial("ws://"+testingHostAddress+"/ticker", 10*time.Millisecond)
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	_, err = s.Read(make([]byte, 1))
	if err != ErrTimeout {
		t.Errorf("expected timeout error from Read, got: %s", err)
	}

	err = s.Close()
	if err != ErrTimeout {
		t.Errorf("expected timeout error from Close, got: %s", err)
	}
}
