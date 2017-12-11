package main

import (
	"log"
	"math/rand"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
)

func main() {
	address := os.Getenv("INFLUXDB_SERVER")
	user := os.Getenv("INFLUXDB_USER")
	password := os.Getenv("INFLUXDB_PASSWORD")
	database := os.Getenv("INFLUXDB_DATABASE")
	measurement := os.Getenv("INFLUXDB_MEASUREMENT")

	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr:     address,
		Username: user,
		Password: password,
	})
	if err != nil {
		log.Fatal(err)
	}
	defer c.Close()

	if err := insert(c, database, measurement); err != nil {
		log.Fatal(err)
	}
}

func insert(c client.Client, database string, measurement string) error {
	bp, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  database,
		Precision: "ms",
	})
	if err != nil {
		return err
	}

	value := 1000000
	prevTimestamp := time.Now()

	for i := 0; i < 100000; i++ {
		// Generate a unique timestamp by inserting an artificial millisecond
		// if needed. Maybe we should sleep if the discrepancy from the real
		// clock gets too large.
		now := time.Now()
		timestamp := prevTimestamp.Add(time.Millisecond)
		if now.After(timestamp) {
			timestamp = now
		}
		prevTimestamp = timestamp

		tags := map[string]string{
			"what": "random",
		}
		fields := map[string]interface{}{
			"value": value,
		}
		pt, err := client.NewPoint(measurement, tags, fields, timestamp)
		if err != nil {
			return err
		}
		bp.AddPoint(pt)

		value += 3 - rand.Intn(7)
	}
	c.Write(bp)

	return nil
}
