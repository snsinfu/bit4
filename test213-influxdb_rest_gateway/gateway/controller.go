package main

import (
	"net/http"
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
