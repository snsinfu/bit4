package main

import (
	"fmt"
	"log"
	"net/http"
	"time"

	"github.com/snsinfu/bit4/test213-influxdb_rest_gateway/gateway/app"
)

func main() {
	config, err := app.GetConfig()
	if err != nil {
		log.Fatal(err)
	}

	if err := ping(config); err != nil {
		log.Fatal(err)
	}
}

func ping(config *app.Config) error {
	res, err := tryGet("http://" + config.DatabaseAddress + "/ping", 3)
	if err != nil {
		return err
	}
	defer res.Body.Close()

	log.Println("ping response:", res)

	if res.StatusCode != http.StatusNoContent {
		return fmt.Errorf("unexpected ping status: %d", res.StatusCode)
	}
	return nil
}

func tryGet(url string, retry int) (res *http.Response, err error) {
	for retry >= 0 {
		res, err = http.Get(url)
		if err == nil {
			return res, nil
		}
		retry--
		<-time.After(5 * time.Second)
	}
	return res, err
}
