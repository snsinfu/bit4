package main

import (
	"fmt"
	"net"
	"os"
	"time"

	"../test023-taskchan/taskch"
)

const (
	serverAddress = "localhost:5000"
	proxyAddress  = "localhost:5500"
	bufferSize    = 1472
)

func main() {
	tasks := taskch.New()

	tasks.Go(serverMain)
	tasks.Go(proxyMain)
	tasks.Go(clientMain)
	tasks.Go(clientMain)

	if err := tasks.Wait(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func proxyMain() error {
	proxyAddr, err := net.ResolveUDPAddr("udp", proxyAddress)
	if err != nil {
		return err
	}

	serverAddr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	sock, err := net.ListenUDP("udp", proxyAddr)
	if err != nil {
		return err
	}
	defer sock.Close()

	conn, err := net.DialUDP("udp", nil, serverAddr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, bufferSize)

	for {
		n, peer, err := sock.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if _, err := conn.Write(buf[:n]); err != nil {
			return err
		}

		fmt.Printf("[proxy] from %s\n", peer)
	}
}

func serverMain() error {
	addr, err := net.ResolveUDPAddr("udp", serverAddress)
	if err != nil {
		return err
	}

	sock, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer sock.Close()

	buf := make([]byte, bufferSize)

	for {
		n, peer, err := sock.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		fmt.Printf("[server] %s '%s'\n", peer, buf[:n])
	}
}

func clientMain() error {
	addr, err := net.ResolveUDPAddr("udp", proxyAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	myAddr := conn.LocalAddr().(*net.UDPAddr)
	msg := []byte("The quick brown fox jumps over the lazy dog.")

	for _ = range time.NewTicker(8 * time.Second).C {
		if _, err := conn.Write(msg); err != nil {
			return err
		}

		fmt.Printf("[client] (%s) sent\n", myAddr)
	}

	return nil
}
