package main

import (
	"github.com/labstack/echo"
)

func main() {
	e := echo.New()

	e.POST("/trade/:market/:pair", postTrade)
	e.GET("/chart/:market/:pair", getChart)

	e.Logger.Fatal(e.Start(":1234"))
}
