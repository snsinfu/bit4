package main

import (
	"net/http"
	"os"
	"time"

	"github.com/influxdata/influxdb/client/v2"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(bind("influx", newClient()))

	e.GET("/ping", handlePing)

	e.Logger.Fatal(e.Start(os.Getenv("GATEWAY_ADDRESS")))
}

func bind(key string, value interface{}) echo.MiddlewareFunc {
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			c.Set(key, value)
			return next(c)
		}
	}
}

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
