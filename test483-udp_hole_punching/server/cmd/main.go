package main

import (
	"log"
	"os"

	"github.com/snsinfu/bit4/test483-udp_hole_punching/server"
)

func main() {
	if err := server.Start(os.Args[1]); err != nil {
		log.Fatal(err)
	}
}
