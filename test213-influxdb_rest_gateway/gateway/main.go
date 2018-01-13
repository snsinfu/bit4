package main

import (
	"os"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(bind("influx", newClient()))

	e.GET("/ping", handlePing)
	e.POST("/temperature", handleTemperaturePost)

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
