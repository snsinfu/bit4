package main

import (
	"fmt"
)

func main() {
	s := "22/tcp"

	var port int
	var proto string
	n, err := fmt.Sscanf(s, "%d/%s", &port, &proto)

	fmt.Println(n, err)
	fmt.Println(port, proto)
}
