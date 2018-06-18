package main

import (
	"fmt"
	"net"
	"os"
	"strings"
	"sync/atomic"
	"time"
)

const (
	bufferSize   = 1472
	proxyTimeout = 3 * time.Second
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	if len(os.Args) != 3 {
		return fmt.Errorf("usage: proxy <proxy-addr> <forward-addr>")
	}

	proxyAddr, err := net.ResolveUDPAddr("udp", os.Args[1])
	if err != nil {
		return err
	}

	forwardAddr, err := net.ResolveUDPAddr("udp", os.Args[2])
	if err != nil {
		return err
	}

	return proxy(proxyAddr, forwardAddr)
}

type Session struct {
	serverConn *net.UDPConn
	clientConn *net.UDPConn
	clientAddr *net.UDPAddr
	timeout    int32
}

func proxy(proxyAddr, forwardAddr *net.UDPAddr) error {
	listen, err := net.ListenUDP("udp", proxyAddr)
	if err != nil {
		return err
	}
	defer listen.Close()

	sessions := map[string]*Session{}

	for {
		buf := make([]byte, bufferSize)

		n, peer, err := listen.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		peerName := peer.String()

		if sess, ok := sessions[peerName]; !ok || sess.serverConn == nil { // XXX data race?
			server, err := net.DialUDP("udp", nil, forwardAddr)
			if err != nil {
				return err
			}

			sess := &Session{
				serverConn: server,
				clientConn: listen,
				clientAddr: peer,
			}
			sessions[peerName] = sess

			go downlink(sess)
		}

		sess := sessions[peerName]

		if _, err := sess.serverConn.Write(buf[:n]); err != nil {
			return err
		}

		atomic.StoreInt32(&sess.timeout, 0)
	}
}

func downlink(sess *Session) error {
	defer func() {
		sess.serverConn.Close()
		sess.serverConn = nil // XXX data race?
	}()

	buf := make([]byte, bufferSize)

	go func() {
		ticker := time.NewTicker(proxyTimeout / 2)
		defer ticker.Stop()

		for _ = range ticker.C {
			if atomic.AddInt32(&sess.timeout, 1) == 2 {
				sess.serverConn.Close()
				break
			}
		}
	}()

	for {
		n, err := sess.serverConn.Read(buf)
		if err != nil {
			// https://github.com/golang/go/issues/4373
			if strings.Contains(err.Error(), "use of closed network connection") {
				return nil // OK; read canceled due to timeout.
			}
			return err
		}

		atomic.StoreInt32(&sess.timeout, 0)

		if _, err := sess.clientConn.WriteToUDP(buf[:n], sess.clientAddr); err != nil {
			return err
		}
	}
}
