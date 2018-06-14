package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/websocket"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	http.HandleFunc("/session", serveSession)
	return http.ListenAndServe(":4000", nil)
}

func serveSession(w http.ResponseWriter, req *http.Request) {
	upgrader := websocket.Upgrader{}
	ws, err := upgrader.Upgrade(w, req, nil)
	if err != nil {
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusInternalServerError)

		body, _ := json.Marshal(map[string]string{
			"error": err.Error(),
		})
		w.Write(body)

		return
	}
	defer ws.Close()

	msgbuf := make([]byte, 1472)

	addr, err := net.ResolveUDPAddr("udp", "localhost:4000")
	if err != nil {
		log.Println(err)
		return
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		log.Println(err)
		return
	}
	defer conn.Close()

	log.Println("Listening UDP on", addr)

	go func() {
		ticker := time.NewTicker(5 * time.Second)

		for _ = range ticker.C {
			conn, err := net.DialUDP("udp", nil, addr)
			if err != nil {
				return
			}
			defer conn.Close()

			conn.Write([]byte("Lorem ipsum"))
			conn.Write([]byte("dolor sit amet"))
		}
	}()

	for {
		n, peer, err := conn.ReadFromUDP(msgbuf)
		if err != nil {
			log.Println(err)
			break
		}
		log.Println("Got", n, "bytes from peer", peer)

		msg := msgbuf[:n]
		if err := ws.WriteMessage(websocket.BinaryMessage, msg); err != nil {
			log.Println(err)
			break
		}
	}

	ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseNormalClosure, ""),
		time.Now().Add(5 * time.Second),
	)
}
