package main

import (
	"net/http"

	"github.com/labstack/echo"
)

// POST "/trade/:market/:pair"
func postTrade(c echo.Context) error {
	return c.String(http.StatusOK, "")
}
