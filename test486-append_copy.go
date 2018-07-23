package main

import (
	"fmt"
)

func main() {
	a := []byte{1, 2, 3}
	b := append([]byte{}, a...)

	a[1] = 100

	fmt.Println(a)
	fmt.Println(b)
}
