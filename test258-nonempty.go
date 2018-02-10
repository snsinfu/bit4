package main

import (
	"errors"
	"fmt"
	"os"
)

var (
	ErrEmpty = errors.New("string is empty")
)

func main() {
	port, _ := NonEmpty(os.Getenv("PORT"), "80")
	fmt.Println(port)
}

// NonEmpty returns the first non-empty string in the argument list. Returns
// an empty string and ErrEmpty if all strings are empty.
func NonEmpty(strs... string) (string, error) {
	for _, str := range strs {
		if len(str) > 0 {
			return str, nil
		}
	}
	return "", ErrEmpty
}
