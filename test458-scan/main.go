package main

import (
	"fmt"

	"github.com/snsinfu/bit4/test458-scan/scanner"
)

func main() {
	sc := scanner.New("443/tcp")

	port := sc.ScanInt()
	delim := sc.Consume('/')
	proto := sc.ScanIdent()

	if err := sc.Stop(); err != nil {
		fmt.Println("error:", err)
	}

	fmt.Println("port:", port)
	fmt.Println("delim:", delim)
	fmt.Println("proto:", proto)
}
