package main

import (
	"net/http"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/session/tcp/:id", GetTCPSession)
	e.GET("/session/udp/:id", GetUDPSession)

	e.Logger.Fatal(e.Start(":9000"))
}

func GetTCPSession(c echo.Context) error {
	return c.String(http.StatusOK, c.Param("id"))
}

func GetUDPSession(c echo.Context) error {
	return c.String(http.StatusOK, c.Param("id"))
}
