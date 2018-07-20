package server

import (
	"log"
	"net"
)

func Start(saddr string) error {
	addr, err := net.ResolveUDPAddr("udp", saddr)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		return err
	}
	defer conn.Close()

	log.Printf("Listening on %s", addr)

	peersStore := map[string][]*net.UDPAddr{}

	buf := make([]byte, 128)
	for {
		n, peer, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		key := string(buf[:n])
		log.Printf("Client %s connected to %q", peer, key)

		peers, _ := peersStore[key]
		peers = append(peers, peer)

		if len(peers) == 2 {
			log.Printf("Pairing %q: %s and %s", key, peers[0], peers[1])

			conn.WriteToUDP([]byte(peers[0].String()), peers[1])
			conn.WriteToUDP([]byte(peers[1].String()), peers[0])

			peers = []*net.UDPAddr{}
		}

		peersStore[key] = peers
	}
}
