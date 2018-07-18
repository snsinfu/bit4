package server

import (
	"net"
)

const (
	listenAddress = ":1111"
)

func Start() error {
	addr, err := net.ResolveUDPAddr("udp4", listenAddress)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp4", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	buf := make([]byte, 1)
	for {
		_, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		resp := []byte(peer.String())
		if _, err:= conn.WriteToUDP(resp, peer); err != nil {
			return err
		}
	}
}
