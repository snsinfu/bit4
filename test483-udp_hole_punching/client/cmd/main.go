package main

import (
	"log"
	"os"

	"github.com/snsinfu/bit4/test483-udp_hole_punching/client"
)

func main() {
	if err := client.Start(os.Args[1], os.Args[2], os.Args[3]); err != nil {
		log.Fatal(err)
	}
}
