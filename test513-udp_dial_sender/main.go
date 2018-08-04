package main

// How Dial-started connection handles UDP messages sent from unknown peers?
//
// -> It blocks. These "unauthorized" messages seem to be blocked by the kernel.
// Wireshark detects ICMP port unreachable errors. Neat.
//
// I believe this is a feature of the "destination-specific datagram connection"
// noted in the source code [1], which is created by the connect syscall [2].
// Indeed, the specification of the connect syscall [3] says that it "limits the
// remote sender," so this should be a specified behavior.
//
// [1]: https://github.com/golang/go/blob/release-branch.go1.10/src/net/sock_posix.go#L64
// [2]: https://github.com/golang/go/blob/release-branch.go1.10/src/net/sock_posix.go#L142
// [3]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/connect.html

import (
	"log"
	"net"
	"time"

	"github.com/snsinfu/go-taskch"
)

func main() {
	tasks := taskch.New()

	// Server sends messages to the first client
	serverCh := make(chan *net.UDPAddr)

	tasks.Go(func() error {
		conn, err := net.ListenUDP("udp4", nil)
		if err != nil {
			return err
		}
		defer conn.Close()

		addr := conn.LocalAddr().(*net.UDPAddr)
		serverCh <- addr
		log.Print("<server> Started on ", addr)

		buf := make([]byte, 1472)

		n, client, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		log.Printf("<server> %s: (%d) %q", client, n, buf[:n])

		for range time.Tick(time.Second) {
			if _, err := conn.WriteToUDP([]byte("tick"), client); err != nil {
				return err
			}
		}
		return nil
	})

	server := <-serverCh

	// Client communicstes with the server using a socket created in "dial" mode
	clientCh := make(chan *net.UDPAddr)

	tasks.Go(func() error {
		conn, err := net.DialUDP("udp4", nil, server)
		if err != nil {
			return err
		}
		defer conn.Close()

		addr := conn.LocalAddr().(*net.UDPAddr)
		clientCh <- addr
		log.Print("<client> Started on ", addr)

		if _, err := conn.Write([]byte("hello")); err != nil {
			return err
		}

		buf := make([]byte, 1472)
		for {
			n, sender, err := conn.ReadFromUDP(buf)
			if err != nil {
				return err
			}
			log.Printf("<client> %s: (%d) %q", sender, n, buf[:n])
		}
	})

	client := <-clientCh

	// A third party sends messages to the client
	tasks.Go(func() error {
		conn, err := net.ListenUDP("udp4", nil)
		if err != nil {
			return err
		}
		defer conn.Close()

		log.Print("<other> Started on ", conn.LocalAddr())

		for range time.Tick(time.Second) {
			if _, err := conn.WriteToUDP([]byte("yo"), client); err != nil {
				return err
			}
		}
		return nil
	})

	if err := tasks.Wait(); err != nil {
		log.Fatal("error: ", err)
	}
}
