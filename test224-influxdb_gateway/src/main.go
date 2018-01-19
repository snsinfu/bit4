package main

import (
	"os"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
	"github.com/snsinfu/bit4/test224-influxdb_gateway/src/action"
	"github.com/snsinfu/bit4/test224-influxdb_gateway/src/domain"
)

func main() {
	e := echo.New()
	d := domain.New(makeInfluxClient())

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(action.Binder(d))

	e.GET("/ping", action.GetPing)

	e.Logger.Fatal(e.Start(os.Getenv("GATEWAY_ADDRESS")))
}
