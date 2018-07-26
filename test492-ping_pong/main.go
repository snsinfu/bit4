package main

import (
	"log"
	"net"
	"time"

	"github.com/snsinfu/go-taskch"
)

const (
	addrA = "127.0.0.1:12345"
	addrB = "127.0.0.1:54321"
)

func main() {
	tasks := taskch.New()

	tasks.Go(func() error {
		return start(addrA, addrB)
	})

	time.Sleep(3 * time.Second)

	tasks.Go(func() error {
		return start(addrB, addrA)
	})

	if err := tasks.Wait(); err != nil {
		log.Fatal("error: ", err)
	}
}

func start(myAddr, peerAddr string) error {
	me, err := net.ResolveUDPAddr("udp", myAddr)
	if err != nil {
		return err
	}

	peer, err := net.ResolveUDPAddr("udp", peerAddr)
	if err != nil {
		return err
	}

	conn, err := net.ListenUDP("udp", me)
	if err != nil {
		return err
	}
	defer conn.Close()

	// Ping/pong synchronization

	ping := [1]byte{0x80}
	pong := [1]byte{0x81}
	pingCh := make(chan bool)

	go func() {
		tick := time.Tick(time.Second)

		conn.WriteToUDP(ping[:], peer)

	pingLoop:
		for {
			select {
			case <-tick:
				log.Printf("%s: o ping -> %s", me, peer)
				conn.WriteToUDP(ping[:], peer)

			case <-pingCh:
				break pingLoop
			}
		}

		log.Printf("%s: ! pong -> %s", me, peer)
		conn.WriteToUDP(pong[:], peer)
	}()

	pinged := false
syncLoop:
	for {
		buf := [1]byte{}

		_, from, err := conn.ReadFromUDP(buf[:])
		if err != nil {
			return err
		}
		if from.String() != peer.String() {
			continue
		}

		switch buf {
		case ping:
			if !pinged {
				close(pingCh)
			}
			pinged = true

		case pong:
			log.Printf("%s: Got pong", me)
			break syncLoop

		default:
		}
	}

	// Communication
	if _, err := conn.WriteToUDP([]byte("Hello!"), peer); err != nil {
		return err
	}
	time.Sleep(time.Second)

	buf := make([]byte, 1472)

	n, from, err := conn.ReadFromUDP(buf)
	if err != nil {
		return err
	}

	log.Printf("%s: Got %q from %s", me, buf[:n], from)

	return nil
}
