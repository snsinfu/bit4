package main

import (
	"fmt"
)

func main() {
	src := "service: 80/tcp"
	port := 0
	proto := ""

	if err := Scan(src, "service", ':', &port, '/', &proto); err != nil {
		fmt.Println("error:", err)
	}
	fmt.Println(port)
}
