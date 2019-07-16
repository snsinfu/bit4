package main

import (
	"net/http"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Recover())
	e.Use(middleware.Logger())

	e.Use(middleware.BasicAuthWithConfig(middleware.BasicAuthConfig{
		Realm:     "Jupyterlab Proxy",
		Validator: func(username, _ string, c echo.Context) (bool, error) {
			c.Set("username", username)
			req := c.Request()
			req.Header.Del("Authorization")
			return true, nil
		},
	}))

	e.GET("/", func(c echo.Context) error {
		username := c.Get("username").(string)
		return c.JSON(http.StatusOK, map[string]interface{}{
			"username": username,
			"header":   c.Request().Header,
		})
	})

	e.Logger.Fatal(e.Start("localhost:3050"))
}
