package main

import (
	"fmt"
	"math"
)

func main() {
	fmt.Println(math.Mod(12.3, 6))
	fmt.Println(math.Mod(-4.5, 6))

	fmt.Println(posMod(12.3, 6))
	fmt.Println(posMod(-4.5, 6))

	fmt.Println(math.Remainder(12.3, 6))
	fmt.Println(math.Remainder(-4.5, 6))
}

func posMod(x, m float64) float64 {
	y := math.Mod(x, m)
	if y < 0 {
		return y + m
	}
	return y
}
