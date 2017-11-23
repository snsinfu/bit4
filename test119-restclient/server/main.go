package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
	"time"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func run() error {
	PORT := os.Getenv("PORT")

	mux := http.NewServeMux()
	mux.Handle("/v1/posts", &postHandler{})

	server := &http.Server{
		Addr:    ":" + PORT,
		Handler: mux,
	}
	defer server.Close()

	return server.ListenAndServe()
}

type postHandler struct{}

func (h *postHandler) ServeHTTP(res http.ResponseWriter, req *http.Request) {
	key := req.Header.Get("X-Auth-Key")
	timestamp := req.Header.Get("X-Auth-Timestamp")
	signature := req.Header.Get("X-Auth-Signature")

	type Response struct {
		Message string `json:"message"`
	}

	if err := validateTimestamp(timestamp); err != nil {
		res.Write([]byte(`{"message": "Bad timestamp"}`))
		return
	}

	secret, err := getSecret(key)
	if err != nil {
		res.Write([]byte(`{"message": "Bad key"}`))
		return
	}

	message, err := ioutil.ReadAll(req.Body)
	if err != nil {
		res.Write([]byte(`{"message": "Server error"}`))
		return
	}

	mac := hmac.New(sha256.New, secret)
	mac.Write([]byte(timestamp))
	mac.Write([]byte(req.Method))
	mac.Write([]byte(req.URL.Path))
	mac.Write(message)
	expectedSignature := hex.EncodeToString(mac.Sum(nil))

	if signature != expectedSignature {
		res.Write([]byte(`{"message": "Bad signature"}`))
		return
	}

	type Post struct {
		Title string `json:"title"`
		Score int    `json:"score"`
	}
	var post Post
	if err := json.Unmarshal(message, &post); err != nil {
		res.Write([]byte(`{"message": "Bad message"}`))
		return
	}

	res.Write([]byte(`{"message": "OK"}`))
	fmt.Println(post)
}

func validateTimestamp(timestamp string) error {
	now, err := strconv.ParseInt(timestamp, 10, 64)
	if err != nil {
		return errors.New("Bad timestamp format")
	}
	delta := time.Now().Sub(time.Unix(now, 0))
	if delta < 0 || delta > 10*time.Second {
		return errors.New("Bad timestamp value")
	}
	return nil
}

func getSecret(key string) ([]byte, error) {
	repository := map[string][]byte{
		"quickbrownfox": []byte("lazydog"),
	}
	secret, ok := repository[key]
	if !ok {
		return nil, errors.New("Unrecognized key")
	}
	return secret, nil
}
