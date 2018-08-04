package main

import (
	"fmt"
)

func main() {
	var host string
	var port int

	s := newScanner("www.example.com:443")

	s.Scan(&host)
	s.Scan(":")
	s.Scan(&port)

	if err := s.Finish(); err != nil {
		fmt.Println("error:", err)
	}

	fmt.Println("host:", host)
	fmt.Println("port:", port)
}
