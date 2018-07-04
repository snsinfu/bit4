package main

import (
	"errors"
	"net/http"
	"strconv"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

var (
	ErrInvalidPort = errors.New("invalid port")

	allowedPort = map[string]int{
		"loremipsum": 22,
	}
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.GET("/tcp/:port", GetPort, InterpretPort, middleware.KeyAuth(AuthorizePort))

	e.Logger.Fatal(e.Start(":3000"))
}

// GetPort handles GET /tcp/:port request.
func GetPort(c echo.Context) error {
	return Data(c, map[string]interface{}{
		"port": c.Get("port"),
	})
}

// InterpretPort is a middleware that interprets path parameter "port" as an
// valid TCP port number and stores it to the context as "port" data.
func InterpretPort(next echo.HandlerFunc) echo.HandlerFunc {
	return func(c echo.Context) error {
		port, err := strconv.Atoi(c.Param("port"))
		if err != nil {
			return Error(c, ErrInvalidPort)
		}
		if port <= 0 || 65536 <= port {
			return Error(c, ErrInvalidPort)
		}
		c.Set("port", port)

		return next(c)
	}
}

// AuthorizePort is a KeyAuth middleware handler that checks if the client can
// access to the port specified in "port" context data. Requires InterpretPort
// middleware upstream.
func AuthorizePort(key string, c echo.Context) (bool, error) {
	if _, ok := allowedPort[key]; !ok {
		return false, nil
	}

	port := c.Get("port").(int)
	if port != allowedPort[key] {
		return false, nil
	}

	return true, nil
}

// Data responds to a request with JSON data.
func Data(c echo.Context, data interface{}) error {
	return c.JSON(http.StatusOK, data)
}

// Error responds to a request with an error.
func Error(c echo.Context, err error) error {
	switch err {
	case ErrInvalidPort:
		return c.JSON(http.StatusNotFound, map[string]interface{}{
			"message": "invalid port",
		})
	}
	return err
}
