package main

import (
	"fmt"
	"sync/atomic"
	"os"
	"time"
)

func main() {
	useUnique := os.Getenv("USE_UNIQUE") == "yes"

	prev := unixNow()
	for i := 0; i < 10; i++ {
		var timestamp int64
		if useUnique {
			// https://stackoverflow.com/a/14369695/5266681
			for {
				orig := prev
				now := unixNow() // What if the wallclock changed?
				timestamp = orig + 1
				if now > timestamp {
					timestamp = now
				}
				if atomic.CompareAndSwapInt64(&prev, orig, timestamp) {
					break
				}
			}
		} else {
			timestamp = unixNow()
		}
		fmt.Println(timestamp)
	}
}

func unixNow() int64 {
	return time.Now().UnixNano() / 1000000
}
