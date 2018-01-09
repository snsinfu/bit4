package main

import (
	"encoding/json"
	"errors"
	"log"
	"net/http"
	"os"
	"path"
)

func main() {
	port := os.Getenv("TEST214_PORT")
	if len(port) == 0 {
		port = "8080"
	}
	addr := ":" + port

	http.HandleFunc("/node/", makeJsonEndpoint(handleNode))
	log.Println("Started")
	log.Fatalln(http.ListenAndServe(addr, nil))
}

type jsonEndpointFunc func(*http.Request) (interface{}, int)

func makeJsonEndpoint(handler jsonEndpointFunc) http.HandlerFunc {
	return func(res http.ResponseWriter, req *http.Request) {
		body, status := handler(req)
		if body == nil {
			res.WriteHeader(status)
			return
		}

		bodyJSON, err := json.Marshal(body)
		if err != nil {
			res.WriteHeader(http.StatusInternalServerError)
			log.Println(err)
			return
		}

		res.WriteHeader(status)
		res.Write(bodyJSON)
		res.Header().Set("Content-type", "application/json")
	}
}

func handleNode(req *http.Request) (interface{}, int) {
	ok, err := path.Match("/node/*", req.URL.Path)
	if !ok {
		return nil, http.StatusForbidden
	}
	if err != nil {
		return nil, http.StatusInternalServerError
	}

	_, node := path.Split(req.URL.Path)
	state, err := queryNodeState(node)
	if err != nil {
		return nil, http.StatusNotFound
	}

	return map[string]string{"node": node, "state": state}, http.StatusOK
}

func queryNodeState(node string) (string, error) {
	stateDB := map[string]string{
		"globe":   "up",
		"potato":  "up",
		"cowbell": "down",
	}
	state, ok := stateDB[node]
	if !ok {
		return state, errors.New("node does not exist")
	}
	return state, nil
}
