package main

import (
	"fmt"
	"net"
)

func main() {
	addrA, _ := net.ResolveUDPAddr("udp", "127.0.0.1:1")
	addrB, _ := net.ResolveUDPAddr("udp", "127.0.0.1:2")
	addrC, _ := net.ResolveUDPAddr("udp", "127.0.0.1:2")

	fmt.Println(equal(addrA, addrA))
	fmt.Println(equal(addrA, addrB))
	fmt.Println(equal(addrA, addrC))
	fmt.Println(equal(addrB, addrB))
	fmt.Println(equal(addrB, addrC))
	fmt.Println(equal(addrC, addrC))
}

func equal(a, b *net.UDPAddr) bool {
	return a.Port == b.Port && a.IP.Equal(b.IP) && a.Zone == b.Zone
}
