package main

import (
	"encoding/base64"
	"fmt"
	"log"
	"net"
	"net/http"
	"strconv"
	"strings"
	"sync/atomic"
	"time"

	"github.com/gorilla/websocket"
	"github.com/labstack/echo"
)

const (
	bufferSize       = 1472
	sessionTimeout   = 30 * time.Second
	websocketTimeout = 5 * time.Second
)

var (
	upgrader = websocket.Upgrader{}
	sessions = map[string]*Session{}
)

type Session struct {
	listener *net.UDPConn
	client   *net.UDPAddr
	ws       *websocket.Conn
	timeout  int32
}

func handleGetUDPPort(c echo.Context) error {
	port, err := strconv.Atoi(c.Param("port"))
	if err != nil {
		return err
	}

	eventSock, err := upgrader.Upgrade(c.Response(), c.Request(), nil)
	if err != nil {
		return err
	}

	go func() {
		defer eventSock.Close()

		if err := startUDPProxy(eventSock, port); err != nil {
			c.Logger().Error(err)
		}
	}()

	return nil
}

func startUDPProxy(eventSock *websocket.Conn, port int) error {
	addr, err := net.ResolveUDPAddr("udp", fmt.Sprintf(":%d", port))
	if err != nil {
		return err
	}

	listener, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer listener.Close()

	log.Printf("Listening on %s/udp", addr)

	buf := make([]byte, bufferSize)

	for {
		n, client, err := listener.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		id := makeSessionID(client)

		if _, ok := sessions[id]; !ok {
			session := &Session{
				listener: listener,
				client:   client,
			}
			sessions[id] = session

			eventSock.WriteJSON(map[string]interface{}{
				"event":      "connect",
				"client_ip":  client.IP,
				"session_id": id,
			})
		}

		// Uplink
		ws := sessions[id].ws

		if ws == nil {
			// Drop the message.
			continue
		}

		if err := ws.WriteMessage(websocket.BinaryMessage, buf[:n]); err != nil {
			return err
		}
	}
}

func makeSessionID(client *net.UDPAddr) string {
	return base64.URLEncoding.EncodeToString([]byte(client.String()))
}

func handleGetSession(c echo.Context) error {
	sessionID := c.Param("id")

	session, ok := sessions[sessionID]
	if !ok {
		return c.JSON(http.StatusNotFound, map[string]string{
			"error": "invalid session ID",
		})
	}

	ws, err := upgrader.Upgrade(c.Response(), c.Request(), nil)
	if err != nil {
		return err
	}

	session.ws = ws

	go func() {
		if err := startSession(session); err != nil {
			c.Logger().Error(err)
		}
	}()

	return nil
}

func startSession(session *Session) error {
	defer func() {
		closeWebSocket(session.ws)
	}()

	// Timeout
	go func() {
		ticker := time.NewTicker(sessionTimeout / 2)
		defer ticker.Stop()

		for _ = range ticker.C {
			if atomic.AddInt32(&session.timeout, 1) == 2 {
				closeWebSocket(session.ws)
				break
			}
		}
	}()

	// Downlink
	buf := make([]byte, bufferSize)

	for {
		_, r, err := session.ws.NextReader()
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

		atomic.StoreInt32(&session.timeout, 0)

		if _, err := session.listener.WriteToUDP(buf[:n], session.client); err != nil {
			return err
		}
	}
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
