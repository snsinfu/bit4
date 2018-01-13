package main

import (
	"log"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
)

const pingTimeout = 1 * time.Second

func newClient() client.Client {
	c, err := client.NewHTTPClient(makeClientConfig())
	if err != nil {
		log.Fatal(err)
	}

	log.Print("Ping: ", testPing(c))

	return c
}

func testPing(c client.Client) string {
	_, msg, err := c.Ping(pingTimeout)
	if err != nil {
		log.Fatal(err)
	}
	return msg
}

func makeClientConfig() client.HTTPConfig {
	var config client.HTTPConfig

	if addr, ok := os.LookupEnv("GATEWAY_INFLUXDB_URL"); ok {
		config.Addr = addr
	} else {
		config.Addr = "http://:8086"
	}

	if user, ok := os.LookupEnv("GATEWAY_INFLUXDB_USER"); ok {
		config.Username = user
	}
	if pass, ok := os.LookupEnv("GATEWAY_INFLUXDB_PASSWORD"); ok {
		config.Password = pass
	}

	return config
}
