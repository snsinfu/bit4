package main

import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		header := w.Header()
		header.Set("Content-Type", "application/json")

		body, err := json.Marshal(map[string]interface{}{
			"msg": "ok",
		})
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		w.WriteHeader(http.StatusOK)
		w.Write(body)
	})
	return http.ListenAndServe(":"+os.Getenv("PORT"), nil)
}
