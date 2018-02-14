package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"time"
)

const (
	defaultPort    = "4040"
	acceptInterval = 10 * time.Second
)

func main() {
	port := os.Getenv("PORT")
	if port == "" {
		port = defaultPort
	}

	a := action{
		limiter: time.NewTicker(acceptInterval),
	}
	http.HandleFunc("/", a.serve)

	log.Print("Listening on " + port)
	log.Fatal(http.ListenAndServe(":" + port, nil))
}

type action struct {
	limiter *time.Ticker
}

func (a *action) serve(res http.ResponseWriter, req *http.Request) {
	<-a.limiter.C

	fmt.Fprintln(res, "OK")
}
