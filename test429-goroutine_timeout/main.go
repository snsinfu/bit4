package main

import (
	"fmt"
	"net"
	"os"
	"time"

	"../test023-taskchan/taskch"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	tasks := taskch.New()

	tasks.Go(func() error {
		conn, err := net.Dial("tcp", "localhost:4000")
		if err != nil {
			return err
		}
		defer conn.Close()

		timeout := time.After(5 * time.Second)
		go func() {
			<-timeout
			conn.Close()
		}()

		buf := make([]byte, 1)
		if _, err := conn.Read(buf); err != nil {
			return err
		}

		return nil
	})

	return tasks.Wait()
}
