package main

import (
	"fmt"
	"log"
	"math/rand"
	"time"

	"github.com/influxdata/influxdb/client/v2"
)

const (
	address  = "http://localhost:8086"
	user     = "alice"
	password = "alice"
	DB       = "testdb"
)

func main() {
	ping()
	clear()
	write()
	read()
}

func ping() {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr: address,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	pingTime, _, err := c.Ping(3 * time.Second)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Ping time:", pingTime)
}

func clear() {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr: address,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	q := client.NewQuery("DELETE FROM trade", DB, "s")
	r, err := c.Query(q)
	if err != nil {
		log.Fatal(err)
	}
	if len(r.Err) > 0 {
		log.Fatal(r.Err)
	}
}

func write() {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     address,
		Username: user,
		Password: password,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	bp, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  DB,
		Precision: "ms",
	})
	if err != nil {
		log.Fatal(err)
	}

	pairs := []string{
		"btc_eth", "btc_ltc", "btc_bch",
	}
	markets := []string{
		"ax", "bx", "cx",
	}
	positions := []string{
		"sell", "buy",
	}

	log.Println("Generating data points")

	price := 0.01
	momentum := 0.001

	for i := 0; i < 100; i++ {
		tags := map[string]string{
			"pair":     pairs[rand.Intn(len(pairs))],
			"market":   markets[rand.Intn(len(markets))],
			"position": positions[rand.Intn(len(positions))],
		}
		fields := map[string]interface{}{
			"price":  price * (0.999 + 0.002*rand.Float64()),
			"amount": rand.ExpFloat64(),
		}

		p, err := client.NewPoint("trade", tags, fields, time.Now().UTC())
		if err != nil {
			log.Fatal(err)
		}
		bp.AddPoint(p)

		<-time.NewTimer(123 * time.Millisecond).C
		nextPrice := momentum + 0.0001*rand.NormFloat64()
		momentum += 0.01 * (nextPrice - price)
		price = nextPrice
	}

	log.Println("Sending the data in batch")

	if err := c.Write(bp); err != nil {
		log.Fatal(err)
	}

	log.Println("Data sent")
}

func read() {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     address,
		Username: user,
		Password: password,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	log.Println("Querying data")

	// Bind parameters are sent as JSON, so durations cannot be bound. We have
	// to embed duration parameters using Sprintf(). Ugh!
	span := "10m"
	bin := "1s"
	qs := fmt.Sprintf(
		"SELECT sum(weighted_price) / sum(amount) AS mean_price"+
			" FROM (SELECT price * amount AS weighted_price, amount"+
			"         FROM trade"+
			"        WHERE market = $market"+
			"          AND pair = $pair"+
			"          AND time > now() - %s)"+
			" GROUP BY time(%s) fill(none)",
		span, bin)

	q := client.NewQueryWithParameters(qs, DB, "s", map[string]interface{}{
		"market": "bx",
		"pair":   "btc_eth",
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
			fmt.Println(row.Columns)
			for _, value := range row.Values {
				fmt.Println(value)
			}
		}
	}
}
