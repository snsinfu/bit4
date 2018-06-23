package wsock

import (
	"net/http"
	"os"
	"testing"
	"time"

	"github.com/gorilla/websocket"
)

const (
	testingHostAddress = "localhost:14423"
)

func TestMain(m *testing.M) {
	os.Exit(func() int {
		server := setupTestingServer(testingHostAddress)
		go server.ListenAndServe()
		defer server.Close()

		// HACK: Wait for the server to start up
		time.Sleep(10 * time.Millisecond)

		return m.Run()
	}())
}

func setupTestingServer(addr string) *http.Server {
	mux := http.NewServeMux()
	upgrader := websocket.Upgrader{}

	mux.HandleFunc("/echo", func(w http.ResponseWriter, r *http.Request) {
		ws, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}
		defer ws.Close()

		typ, msg, _ := ws.ReadMessage()
		ws.WriteMessage(typ, msg)
	})

	mux.HandleFunc("/ticker", func(w http.ResponseWriter, r *http.Request) {
		ws, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}
		defer ws.Close()

		ticker := time.NewTicker(5 * time.Second)
		defer ticker.Stop()

		for _ = range ticker.C {
			if err := ws.WriteMessage(websocket.BinaryMessage, []byte{1}); err != nil {
				break
			}
		}
	})

	return &http.Server{
		Addr:    addr,
		Handler: mux,
	}
}
