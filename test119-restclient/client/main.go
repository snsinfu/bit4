package main

import (
	"bytes"
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
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
	API_SERVER := os.Getenv("API_SERVER")
	API_KEY := os.Getenv("API_KEY")
	API_SECRET := []byte(os.Getenv("API_SECRET"))

	type Post struct {
		Title string `json:"title"`
		Score int    `json:"score"`
	}
	post := Post{
		Title: "Lorem ipsum dolor sit amet",
		Score: 97,
	}

	// Compose a request.
	timestamp := strconv.FormatInt(time.Now().Unix(), 10)

	message, err := json.Marshal(&post)
	if err != nil {
		return err
	}

	method := "POST"
	path := "/v1/posts"
	req, err := http.NewRequest(method, API_SERVER+path, bytes.NewReader(message))
	if err != nil {
		return err
	}

	mac := hmac.New(sha256.New, API_SECRET)
	mac.Write([]byte(timestamp))
	mac.Write([]byte(method))
	mac.Write([]byte(path))
	mac.Write(message)
	signature := hex.EncodeToString(mac.Sum(nil))

	req.Header.Add("X-Auth-Key", API_KEY)
	req.Header.Add("X-Auth-Timestamp", timestamp)
	req.Header.Add("X-Auth-Signature", signature)
	req.Header.Add("Content-Type", "application/json")

	// Send the request.
	client := &http.Client{}
	res, err := client.Do(req)
	if err != nil {
		return err
	}
	defer res.Body.Close()

	if err := checkResponse(res); err != nil {
		return err
	}

	// Examine the response.
	type Status struct {
		Message string `json:"message"`
	}
	var stat Status
	if err := parseJsonResponse(res, &stat); err != nil {
		return err
	}
	fmt.Println(stat.Message)

	return nil
}

func checkResponse(res *http.Response) error {
	if res.StatusCode/100 != 2 {
		return fmt.Errorf("HTTP status %d", res.StatusCode)
	}
	return nil
}

func parseJsonResponse(res *http.Response, data interface{}) error {
	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		return err
	}
	return json.Unmarshal(body, data)
}
