package main

import "fmt"

func main() {
	var kvs map[string]int

	for k, v := range kvs {
		fmt.Println(k, v)
	}
}
