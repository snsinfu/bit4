package main

import (
	"encoding/json"
	"fmt"
	"os"
	"time"
)

func main() {
	var e struct {
		Time time.Time `json:"time"`
	}
	src := []byte(`{"time": "2018-11-14T18:54:37.611Z"}`)

	if err := json.Unmarshal(src, &e); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}

	fmt.Println("Time:", e.Time)
}
