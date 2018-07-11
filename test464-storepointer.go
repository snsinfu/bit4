package main

import (
	"fmt"
	"sync/atomic"
	"unsafe"
)

func main() {
	n := 42

	p := &n
	q := (*int)(nil)
	atomic.StorePointer((*unsafe.Pointer)(unsafe.Pointer(&q)), unsafe.Pointer(p))

	fmt.Println(*q)
}
