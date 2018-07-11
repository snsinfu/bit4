package main

import (
	"sync"
)

type SharedDict struct {
	sync.Mutex
	dict map[string]string
}

func (dict *SharedDict) Set(key, value string) {
	defer dict.Unlock()
	dict.Lock()

	if dict.dict == nil {
		dict.dict = map[string]string{}
	}

	dict.dict[key] = value
}

func (dict *SharedDict) Get(key string) (string, bool) {
	dict.Lock()
	defer dict.Unlock()

	val, ok := dict.dict[key]
	return val, ok
}

func (dict *SharedDict) Remove(key string) {
	dict.Lock()
	defer dict.Unlock()

	delete(dict.dict, key)
}

func main() {
	dict := SharedDict{}

	dict.Set("red", "blue")
	dict.Get("red")
	dict.Remove("red")
	dict.Get("red")
}
