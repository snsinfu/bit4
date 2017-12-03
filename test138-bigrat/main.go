package main

import (
	"errors"
	"fmt"
	"log"
	"math/big"
)

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	a := new(big.Rat)
	if _, ok := a.SetString("0.13598336"); !ok {
		return errors.New("Failed to parse a number")
	}

	b := new(big.Rat)
	if _, ok := b.SetString("1287418.0"); !ok {
		return errors.New("Failed to parse a number")
	}

	product := new(big.Rat)
	product.Mul(a, b)
	fmt.Println("Rat:", product)
	fmt.Println("Rat decimal:", product.FloatString(8))

	scale := new(big.Rat)
	scale.SetString("100000000")
	scale.Mul(scale, product)
	if !scale.IsInt() {
		panic("Broken")
	}
	fmt.Println("Fixed point:", scale.FloatString(0))

	return nil
}
