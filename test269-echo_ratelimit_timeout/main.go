package main

import (
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())
	e.Use(withTimeout(1 * time.Second))

	e.GET("/", handleRoot, withRateLimit(1 * time.Second))

	port, ok := os.LookupEnv("PORT")
	if !ok {
		port = "3000"
	}
	e.Logger.Fatal(e.Start(":" + port))
}

func handleRoot(c echo.Context) error {
	return c.String(http.StatusOK, fmt.Sprintln(time.Now()))
}

func withRateLimit(interval time.Duration) echo.MiddlewareFunc {
	limiter := time.NewTicker(interval)
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			<-limiter.C
			return next(c)
		}
	}
}

func withTimeout(timeout time.Duration) echo.MiddlewareFunc {
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			timer := time.NewTimer(timeout)
			done := make(chan error, 1)

			go func() {
				done <- next(c)
			}()

			select {
			case <-timer.C:
				return c.String(
					http.StatusServiceUnavailable,
					"Service temporarily unavailable",
				)

			case err := <-done:
				return err
			}
		}
	}
}
