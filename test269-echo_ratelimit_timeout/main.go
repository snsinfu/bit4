package main

import (
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

const (
	interval = 1 * time.Second
	timeout  = 2 * time.Second
)

func main() {
	e := echo.New()
	e.Use(middleware.Logger())
	e.GET("/", func(c echo.Context) error {
		return c.String(http.StatusOK, fmt.Sprintln(time.Now()))
	}, withRateLimit(interval, timeout))

	port, ok := os.LookupEnv("PORT")
	if !ok {
		port = "3000"
	}
	e.Logger.Fatal(e.Start(":" + port))
}

func withRateLimit(interval time.Duration, timeout time.Duration) echo.MiddlewareFunc {
	limiter := time.NewTicker(interval)

	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			timer := time.NewTimer(timeout)

			select {
			case <-limiter.C:
				return next(c)

			case <-timer.C:
				return c.String(
					http.StatusServiceUnavailable,
					"Service temporarily unavailable\n",
				)
			}
		}
	}
}
