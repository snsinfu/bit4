package main

import (
	"log"
	"net/http"
	"time"

	"github.com/gorilla/websocket"
)

func main() {
	upgrader := websocket.Upgrader{}

	http.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		ws, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			log.Print("upgrade error: ", err)
		}

		go session(ws)
	})

	log.Fatal(http.ListenAndServe(":4000", nil))
}

func session(ws *websocket.Conn) {
	defer ws.Close()

	ws.SetPingHandler(func(d string) error {
		log.Print("ping: ", d)
		return nil
	})

	for {
		t, msg, err := ws.ReadMessage()
		if err != nil {
			log.Print("read error: ", err)
			return
		}

		log.Printf("message (%d): %s", t, msg)
	}

	ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseNormalClosure, ""),
		time.Now().Add(5 * time.Second),
	)
}
