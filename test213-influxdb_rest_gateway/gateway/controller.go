package main

import (
	"bufio"
	"net/http"
	"os"
	"strconv"
	"time"

	"github.com/influxdata/influxdb/client/v2"
	"github.com/labstack/echo"
)

func handlePing(c echo.Context) error {
	influx := c.Get("influx").(client.Client)

	latency, msg, err := influx.Ping(10 * time.Second)
	if err != nil {
		return err
	}

	return c.JSON(http.StatusOK, map[string]interface{}{
		"latency": float64(latency) / float64(time.Second),
		"message": msg,
	})
}

func handleTemperaturePost(c echo.Context) error {
	batch, err := client.NewBatchPoints(client.BatchPointsConfig{
		Database:  os.Getenv("GATEWAY_INFLUXDB_DATABASE"),
		Precision: "ms",
	})
	if err != nil {
		return err
	}

	tags := map[string]string{
		"via": "gateway",
	}
	fields := map[string]interface{}{
		"temperature": 0.0,
	}
	timestamp := time.Now()

	scanner := bufio.NewScanner(c.Request().Body)
	for scanner.Scan() {
		value, err := strconv.ParseFloat(scanner.Text(), 64)
		if err != nil {
			return err
		}

		fields["temperature"] = value

		pt, err := client.NewPoint("metrics", tags, fields, timestamp)
		if err != nil {
			return err
		}
		batch.AddPoint(pt)

		timestamp = timestamp.Add(time.Millisecond)
	}
	if err := scanner.Err(); err != nil {
		return err
	}

	influx := c.Get("influx").(client.Client)
	if err := influx.Write(batch); err != nil {
		return err
	}

	return c.NoContent(http.StatusOK)
}
