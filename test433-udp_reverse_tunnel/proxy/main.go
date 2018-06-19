package main

import (
	"log"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

const (
	httpListenAddress = "localhost:8000"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/tunnel/udp/:port", handleGetUDPPort)
	e.GET("/session/:id", handleGetSession)

	log.Fatal(e.Start(httpListenAddress))
}
