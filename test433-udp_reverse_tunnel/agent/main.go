package main

import (
	"log"
	"net"
	"time"
	"strings"

	"github.com/gorilla/websocket"
	"github.com/snsinfu/bit4/test023-taskchan/taskch"
)

const (
	proxyBaseURL     = "ws://localhost:8000"
	serverAddress    = "localhost:60000" // mosh
	bufferSize       = 1472
	websocketTimeout = 5 * time.Second
)

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

type ConnectEvent struct {
	Event     string `json:"event"`
	ClientIP  net.IP `json:"client_ip"`
	SessionID string `json:"session_id"`
}

func run() error {
	conn, _, err := websocket.DefaultDialer.Dial(proxyBaseURL+"/tunnel/udp/10000", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	for {
		connect := ConnectEvent{}
		if err := conn.ReadJSON(&connect); err != nil {
			return err
		}

		log.Printf("Connection from %s, session = %s", connect.ClientIP, connect.SessionID)

		go func() {
			if err := startSession(connect.SessionID); err != nil {
				log.Printf("session closed due to: %s", err)
			}
		}()
	}
}

func startSession(id string) error {
	ws, _, err := websocket.DefaultDialer.Dial(proxyBaseURL+"/session/"+id, nil)
	if err != nil {
		return err
	}
	defer closeWebSocket(ws)

	serverAddr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, serverAddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	tasks := taskch.New()

	// Uplink
	tasks.Go(func() error {
		buf := make([]byte, bufferSize)

		for {
			_, r, err := ws.NextReader()
			if err != nil {
				if isClosedError(err) {
					return nil
				}
				return err
			}

			n, err := r.Read(buf)
			if err != nil {
				return err
			}

			if _, err := conn.Write(buf[:n]); err != nil {
				return err
			}
		}
		return nil
	})

	// Downlink
	tasks.Go(func() error {
		buf := make([]byte, bufferSize)

		for {
			n, err := conn.Read(buf)
			if err != nil {
				return err
			}

			if err := ws.WriteMessage(websocket.BinaryMessage, buf[:n]); err != nil {
				return err
			}
		}
		return nil
	})

	return tasks.Wait()
}

func isClosedError(err error) bool {
	return strings.Contains(err.Error(), "use of closed network connection")
}

func closeWebSocket(ws *websocket.Conn) {
	ws.WriteControl(
		websocket.CloseMessage,
		websocket.FormatCloseMessage(websocket.CloseNormalClosure, ""),
		time.Now().Add(websocketTimeout),
	)
	ws.Close()
}
