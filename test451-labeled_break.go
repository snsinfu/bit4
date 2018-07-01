package main

import (
	"fmt"
	"math/rand"
)

func main() {
Loop:
	for {
		n := rand.Intn(100)

		switch {
		case n % 3 == 0:
			fmt.Println(n)

		case n % 7 == 0:
			break Loop
		}
	}
}
