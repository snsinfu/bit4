package main

import (
	"fmt"
	"sync"
)

type Users struct {
	data  map[string]bool
	mutex sync.RWMutex
}

func (u *Users) Lock() {
	u.mutex.Lock()
}

func (u *Users) Unlock() {
	u.mutex.Unlock()
}

func (u *Users) Locked(task func(map[string]bool)) {
	u.Lock()
	task(u.data)
	u.Unlock()
}

func (u *Users) RLock() {
	u.mutex.RLock()
}

func (u *Users) RUnlock() {
	u.mutex.RUnlock()
}

func (u *Users) RLocked(task func(map[string]bool)) {
	u.RLock()
	task(u.data)
	u.RUnlock()
}


func New() Users {
	return Users{
		data: make(map[string]bool),
	}
}

func main() {
	users := New()

	users.Locked(func(us map[string]bool) {
		us["alice"] = true
		us["bob"] = true
	})

	users.RLocked(func(us map[string]bool) {
		for u := range us {
			fmt.Println(u)
		}
	})
}
