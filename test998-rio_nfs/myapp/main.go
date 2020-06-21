package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
)

const counterPath = "/srv/data/counter.txt"

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		counter, err := readCounter()
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		header := w.Header()
		header.Set("Content-Type", "text/plain")

		w.WriteHeader(http.StatusOK)
		fmt.Fprintf(w, "%d\n", counter)
	})
	return http.ListenAndServe(":8000", nil)
}

func readCounter() (int, error) {
	text, err := ioutil.ReadFile(counterPath)
	if err != nil {
		if !os.IsNotExist(err) {
			return 0, err
		}
		text = []byte("0")
	}

	counter, err := strconv.Atoi(string(text))
	if err != nil {
		return 0, err
	}

	counter++
	text = []byte(strconv.Itoa(counter))
	if err := ioutil.WriteFile(counterPath, text, 0666); err != nil {
		return 0, err
	}

	return counter, nil
}
