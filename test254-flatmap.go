package main

import (
	"fmt"
)

func main() {
	source := []int{1, 3, 0, 2, 1}

	ch := flatMap(source, func(n int, out chan<- string) {
		for i := 0; i < n; i++ {
			out <- "foo"
		}
	})

	for r := range ch {
		fmt.Println(r)
	}
}

func flatMap(source []int, f func(int, chan<- string)) <-chan string {
	ch := make(chan string)

	go func() {
		defer close(ch)

		for _, x := range source {
			f(x, ch)
		}
	}()

	return ch
}

