package main

import (
	"log"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
)

func main() {
	c, err := client.NewHTTPClient(makeClientConfig())
	if err != nil {
		log.Fatal(err)
	}

	for i := 0; i < 3; i++ {
		<-time.After(1 * time.Second)
		_, pong, err := c.Ping(10 * time.Second)
		if err != nil {
			log.Fatal(err)
		}
		log.Print("Pong: ", pong)
	}
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
