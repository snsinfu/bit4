package client

import (
	"log"
	"net"
	"time"
)

func Start(source, server, key string) error {
	sourceAddr, err := net.ResolveUDPAddr("udp", source)
	if err != nil {
		return err
	}

	serverAddr, err := net.ResolveUDPAddr("udp", server)
	if err != nil {
		return err
	}

	peerAddr, err := rendezvous(sourceAddr, serverAddr, key)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", sourceAddr)
	if err != nil {
		return err
	}

	go func() {
		log.Print("Punching through...")
		count := 0

		for range time.NewTicker(time.Second).C {
			if _, err := conn.WriteToUDP([]byte("Hello!"), peerAddr); err != nil {
				log.Printf("Hole-punch failed: %s", err)
				conn.Close()
				return
			}

			count++
			if count & (count - 1) == 0 {
				log.Printf("%d hole-punching packets sent", count)
			}
		}
	}()

	log.Print("Waiting for a message...")

	buf := make([]byte, 128)

	for {
		n, _, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		log.Printf("Got message %q", buf[:n])
	}
}

func rendezvous(source, server *net.UDPAddr, key string) (*net.UDPAddr, error) {
	log.Printf("Rendezvous: %s -> %s", source, server)

	conn, err := net.DialUDP("udp", source, server)
	if err != nil {
		return nil, err
	}
	defer conn.Close()

	if _, err := conn.Write([]byte(key)); err != nil {
		return nil, err
	}

	log.Print("Waiting for the peer...")

	buf := make([]byte, 128)

	n, err := conn.Read(buf)
	if err != nil {
		return nil, err
	}

	peer, err := net.ResolveUDPAddr("udp", string(buf[:n]))
	if err != nil {
		return nil, err
	}

	log.Printf("Our peer is %s", peer)

	return peer, nil
}
