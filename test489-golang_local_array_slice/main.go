package main

import "fmt"

func main() {
	a := encode(1234)
	b := encode(5678)
	fmt.Println(a, b)
}

func encode(val int16) []byte {
	buf := [2]byte{
		byte(val >> 16),
		byte(val & 0xFF),
	}
	// Yes, returning local slice is safe.
	// https://stackoverflow.com/questions/42530219/is-returning-a-slice-of-a-local-array-in-a-go-function-safe
	return buf[:]
}
