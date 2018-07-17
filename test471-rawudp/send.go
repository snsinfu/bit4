package main

import (
	"encoding/binary"
	"fmt"
	"net"
	"os"
)

func main() {
	if err := run(os.Args[1], os.Args[2]); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run(s1, s2 string) error {
	srcAddr, err := net.ResolveUDPAddr("udp4", s1)
	if err != nil {
		return err
	}

	destAddr, err := net.ResolveUDPAddr("udp4", s2)
	if err != nil {
		return err
	}

	srcIP := &net.IPAddr{
		IP:   srcAddr.IP,
		Zone: srcAddr.Zone,
	}

	destIP := &net.IPAddr{
		IP:   destAddr.IP,
		Zone: destAddr.Zone,
	}

	conn, err := net.DialIP("ip4:udp", srcIP, destIP)
	if err != nil {
		return err
	}
	defer conn.Close()

	// UDP
	packet := []byte("........Lorem Ipsum")

	binary.BigEndian.PutUint16(packet[0:2], uint16(srcAddr.Port))
	binary.BigEndian.PutUint16(packet[2:4], uint16(destAddr.Port))
	binary.BigEndian.PutUint16(packet[4:6], uint16(len(packet)))

	if _, err := conn.Write(packet); err != nil {
		return err
	}

	return nil
}
