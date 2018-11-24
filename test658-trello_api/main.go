package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	for _, id := range os.Args[1:] {
		if err := archiveCard(id); err != nil {
			return err
		}
	}
	return nil
}

func archiveCard(id string) error {
	params := url.Values{
		"key":    {os.Getenv("TRELLO_API_KEY")},
		"token":  {os.Getenv("TRELLO_TOKEN")},
		"closed": {"true"},
	}

	req, err := http.NewRequest(
		"PUT",
		"https://api.trello.com/1/cards/"+id+"?"+params.Encode(),
		nil,
	)
	if err != nil {
		return err
	}

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return err
	}
	defer res.Body.Close()

	if res.StatusCode/100 != 2 {
		body, _ := ioutil.ReadAll(res.Body)
		return fmt.Errorf("HTTP %d: %s", res.StatusCode, string(body))
	}

	return nil
}
