package main

import (
	"fmt"
	"sync"
)

func main() {
	var m sync.Mutex

	for i := 0; i < 5; i++ {
		func() {
			m.Lock()
			defer m.Unlock()

			fmt.Println(i)
		}()
	}
}
