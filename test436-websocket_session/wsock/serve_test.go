package wsock

import (
	"net/http"
	"reflect"
	"testing"
	"time"

	"github.com/gorilla/websocket"
)

func Test_Session_ServerCommunicatesWithClient(t *testing.T) {
	serverAddress := "localhost:14424"
	firstMessage := []byte{2, 3, 5, 7, 11}
	secondMessage := []byte{13, 17, 19, 23, 29}

	mux := http.NewServeMux()

	mux.HandleFunc("/test", func(w http.ResponseWriter, r *http.Request) {
		s, err := Serve(w, r, 30*time.Second)
		if err != nil {
			t.Errorf("unexpected Upgrade error: %s", err)
		}

		err = s.Write(firstMessage)
		if err != nil {
			t.Errorf("unexpected Write error: %s", err)
		}

		buf := make([]byte, 10)
		n, err := s.Read(buf)
		if err != nil {
			t.Errorf("unexpected Read error: %s", err)
		}

		act := buf[:n]
		exp := secondMessage
		if !reflect.DeepEqual(act, exp) {
			t.Errorf("unexpected message: actual %s, expected %s", act, exp)
		}

		err = s.Close()
		if err != nil {
			t.Errorf("unexpected Close error: %s", err)
		}
	})

	server := &http.Server{
		Addr:    serverAddress,
		Handler: mux,
	}

	go server.ListenAndServe()
	time.Sleep(10 * time.Millisecond)
	defer server.Close()

	ws, _, err := websocket.DefaultDialer.Dial("ws://"+serverAddress+"/test", nil)
	if err != nil {
		t.Errorf("failed to connecting to test server: %s", err)
	}
	defer ws.Close()

	_, msg, err := ws.ReadMessage()
	if err != nil {
		t.Errorf("failed to connecting to test server: %s", err)
	}

	if !reflect.DeepEqual(msg, firstMessage) {
		t.Errorf("unexpected message: actual %s, expected %s", msg, firstMessage)
	}

	err = ws.WriteMessage(websocket.BinaryMessage, secondMessage)
	if err != nil {
		t.Errorf("failed to write to test server: %s", err)
	}

	ws.Close()
}
