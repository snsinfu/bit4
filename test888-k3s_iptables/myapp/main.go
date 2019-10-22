package main

import (
	"crypto/rand"
	"encoding/json"
	"fmt"
	"math/big"
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
	num, err := rand.Int(rand.Reader, big.NewInt(9999))
	if err != nil {
		return err
	}

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		header := w.Header()
		header.Set("Content-Type", "application/json")

		body, err := json.Marshal(map[string]interface{}{
			"msg": "ok",
			"num": num,
		})
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		w.WriteHeader(http.StatusOK)
		w.Write(body)
	})
	return http.ListenAndServe(":80", nil)
}
