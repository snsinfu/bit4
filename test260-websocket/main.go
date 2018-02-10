package main

import (
	"log"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/websocket"
)

var (
	upgrader         = websocket.Upgrader{}
	websocketTimeout = 5 * time.Second
	messageInterval  = 5 * time.Second
	defaultPort      = "1230"
)

func main() {
	port := os.Getenv("PORT")
	if port == "" {
		port = defaultPort
	}
	addr := ":" + port

	http.HandleFunc("/", serve)
	log.Print("Listening on " + addr)
	log.Fatal(http.ListenAndServe(addr, nil))
}

func serve(res http.ResponseWriter, req *http.Request) {
	log.Print("Client connected")

	ws, err := upgrader.Upgrade(res, req, nil)
	if err != nil {
		log.Print(err)
		return
	}
	defer ws.Close()

	// TODO: ping/pong

	for t := range time.NewTicker(messageInterval).C {
		msg, err := t.MarshalText()
		if err != nil {
			log.Print(err)
			return
		}

		if err := ws.WriteMessage(websocket.TextMessage, []byte(msg)); err != nil {
			log.Print(err)
			return
		}
	}

	err = ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseNormalClosure, ""),
		time.Now().Add(websocketTimeout),
	)
	if err != nil {
		log.Print(err)
	}

	log.Print("Done")
}
