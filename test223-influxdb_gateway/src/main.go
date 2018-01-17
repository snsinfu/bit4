package main

import (
	"os"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
	"github.com/snsinfu/bit4/test223-influxdb_gateway/src/domain"
	"github.com/snsinfu/bit4/test223-influxdb_gateway/src/rest"
)

func main() {
	e := echo.New()
	d := domain.New(makeInfluxClient())

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(bind("domain", d))

	e.GET("/ping", rest.GetPing)

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
