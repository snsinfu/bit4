package main

import (
	"gopkg.in/macaron.v1" // See https://go-macaron.com
)

func main() {
	m := macaron.Classic()

	m.Get("/", func() (status int, content string) {
		status = 200
		content = "Hello, Macaron"
		return
	})

	m.Run()
}
