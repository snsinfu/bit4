package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Println("Hello, go")

	for _, arg := range os.Args[1:] {
		fmt.Println("Arg:", arg)
	}
}
