package main

import (
	"fmt"
	"net"
	"os"

	"../test023-taskchan/taskch"
)

const (
	bufferSize = 1472
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

type Route struct {
	uplinkChan chan []byte
}

func proxy(proxyAddr, forwardAddr *net.UDPAddr) error {
	listen, err := net.ListenUDP("udp", proxyAddr)
	if err != nil {
		return err
	}
	defer listen.Close()

	routes := map[string]Route{}

	for {
		buf := make([]byte, bufferSize)

		n, peer, err := listen.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		peerName := peer.String()

		if _, ok := routes[peerName]; !ok {
			uplinkChan := make(chan []byte)

			routes[peerName] = Route{
				uplinkChan: uplinkChan,
			}

			go func() error {
				conn, err := net.DialUDP("udp", nil, forwardAddr)
				if err != nil {
					return err
				}
				defer conn.Close()

				tasks := taskch.New()

				// Uplink
				tasks.Go(func() error {
					for msg := range uplinkChan {
						if _, err := conn.Write(msg); err != nil {
							return err
						}
					}
					return nil
				})

				// Downlink
				tasks.Go(func() error {
					buf := make([]byte, bufferSize)

					for {
						m, err := conn.Read(buf)
						if err != nil {
							return err
						}
						msg := buf[:m]

						if _, err := listen.WriteToUDP(msg, peer); err != nil {
							return err
						}
					}
				})

				fmt.Printf(
					"[proxy ] start forwarding %s -> %s @ %s\n",
					peer,
					forwardAddr,
					conn.LocalAddr().(*net.UDPAddr),
				)

				return tasks.Wait()
			}()
		}

		msg := buf[:n]
		routes[peerName].uplinkChan <- msg
	}
}
