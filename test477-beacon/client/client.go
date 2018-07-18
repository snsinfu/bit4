package client

import (
	"fmt"
	"net"
)

const (
	serverAddress = ":1111"
)

func Start() error {
	addr, err := net.ResolveUDPAddr("udp4", serverAddress)
	if err != nil {
		return err
	}

	conn, err := net.DialUDP("udp4", nil, addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, 32)

	if _, err := conn.Write(buf); err != nil {
		return err
	}

	n, err := conn.Read(buf)
	if err != nil {
		return err
	}

	fmt.Println(string(buf[:n]))

	return nil
}
