package main

import (
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

const LISTEN_ADDRESS = ":1323"

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/lamps", enumLamps)
	e.GET("/lamps/:id", getLamp)
	e.PUT("/lamps/:id", updateLamp)

	e.Logger.Fatal(e.Start(LISTEN_ADDRESS))
}
