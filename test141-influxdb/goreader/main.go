package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
)

func main() {
	address := os.Getenv("INFLUXDB_SERVER")
	user := os.Getenv("INFLUXDB_USER")
	password := os.Getenv("INFLUXDB_PASSWORD")
	database := os.Getenv("INFLUXDB_DATABASE")
	market := os.Getenv("REQUEST_MARKET")
	pair := os.Getenv("REQUEST_PAIR")

	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     address,
		Username: user,
		Password: password,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	// Bind parameters are sent as JSON, so durations cannot be bound. We have
	// to embed duration parameters using Sprintf().
	span := "10000w"
	bin := "1s"
	qs := fmt.Sprintf(`
SELECT sum(weighted_price) / sum(amount) AS mean_price
  FROM (SELECT price * amount AS weighted_price, amount
          FROM trade
         WHERE market = $market
           AND pair = $pair
           AND time > now() - %s)
GROUP BY time(%s) fill(none)`, span, bin)

	q := client.NewQueryWithParameters(qs, database, "s", map[string]interface{}{
		"market": market,
		"pair":   pair,
	})

	resp, err := c.Query(q)
	if err != nil {
		log.Fatal(err)
	}
	if len(resp.Err) > 0 {
		log.Fatal(resp.Err)
	}

	for _, res := range resp.Results {
		for _, row := range res.Series {
			for _, value := range row.Values {
				timestamp, err := value[0].(json.Number).Int64()
				if err != nil {
					log.Fatal(err)
				}
				price, err := value[1].(json.Number).Float64()
				if err != nil {
					log.Fatal(err)
				}
				fmt.Printf("%s\t%.1f\n", time.Unix(timestamp, 0), price)
			}
		}
	}
}
