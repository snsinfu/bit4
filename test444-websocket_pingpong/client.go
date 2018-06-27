package main

import (
	"log"
	"time"

	"github.com/gorilla/websocket"
)

func main() {
	dialer := websocket.DefaultDialer

	ws, _, err := dialer.Dial("ws://localhost:4000/ws", nil)
	if err != nil {
		log.Fatal("dial error:", err)
	}
	defer ws.Close()

	ticker := time.NewTicker(time.Second)

	for i := 0; i < 10; i++ {
		<-ticker.C

		if err := ws.WriteMessage(websocket.PingMessage, []byte("ping")); err != nil {
			log.Print("write error:", err)
		}
	}

	ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseNormalClosure, ""),
		time.Now().Add(5 * time.Second),
	)
}
