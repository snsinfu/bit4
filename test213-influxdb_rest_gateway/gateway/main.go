package main

import (
	"net/http"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

var (
	db client.Client
)

func main() {
	db = newClient()
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/ping", handlePing)

	e.Logger.Fatal(e.Start(os.Getenv("GATEWAY_ADDRESS")))
}

func handlePing(c echo.Context) error {
	latency, msg, err := db.Ping(10 * time.Second)
	if err != nil {
		return err
	}
	return c.JSON(http.StatusOK, map[string]interface{}{
		"latency": float64(latency) / float64(time.Second),
		"message": msg,
	})
}
