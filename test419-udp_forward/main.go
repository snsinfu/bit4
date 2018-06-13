package main

import (
	"errors"
	"fmt"
	"net"
	"os"

	"../test023-taskchan/taskch"
)

const bufferSize = 4096

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error: ", err)
		os.Exit(1)
	}
}

func run() error {
	if len(os.Args) != 3 {
		return errors.New("usage: udp_forward <bind-address> <forward-address>")
	}

	listenAddress := os.Args[1]
	forwardAddress := os.Args[2]

	listenAddr, err := net.ResolveUDPAddr("udp", listenAddress)
	if err != nil {
		return err
	}

	forwardAddr, err := net.ResolveUDPAddr("udp", forwardAddress)
	if err != nil {
		return err
	}

	server, err := net.DialUDP("udp", nil, forwardAddr)
	if err != nil {
		return err
	}
	defer server.Close()

	client, err := net.ListenUDP("udp", listenAddr)
	if err != nil {
		return err
	}
	defer client.Close()

	tasks := taskch.New()

	var clientAddr *net.UDPAddr

	tasks.Go(func() error {
		msgbuf := make([]byte, bufferSize)

		for {
			n, resp, err := client.ReadFromUDP(msgbuf)
			if err != nil {
				return err
			}
			clientAddr = resp

			if _, err := server.Write(msgbuf[:n]); err != nil {
				return err
			}
		}
	})

	tasks.Go(func() error {
		msgbuf := make([]byte, bufferSize)

		for {
			n, err := server.Read(msgbuf)
			if err != nil {
				return err
			}

			if _, err := client.WriteToUDP(msgbuf[:n], clientAddr); err != nil {
				return err
			}
		}
	})

	return tasks.Wait()
}
