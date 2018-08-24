package main

import (
	"fmt"
	"sync"
)

func main() {
	var m sync.Mutex

	for i := 0; i < 5; i++ {
		m.Lock()
		defer m.Unlock() // Not unlocked!

		fmt.Println(i)
	}
}
