package main

import (
	"net/url"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Recover())
	e.Use(middleware.Logger())

	selector := newBackendSelector()
	selector.Backends["foo"] = "localhost:3101"
	selector.Backends["bar"] = "localhost:3102"

	e.Use(middleware.BasicAuthWithConfig(middleware.BasicAuthConfig{
		Realm:     "Jupyterlab Proxy",
		Validator: selector.BasicAuth,
	}))
	e.Use(middleware.Proxy(selector))

	e.Logger.Fatal(e.Start("localhost:3050"))
}

type backendSelector struct {
	Backends map[string]string
}

func newBackendSelector() *backendSelector {
	return &backendSelector{
		Backends: make(map[string]string),
	}
}

func (b *backendSelector) BasicAuth(username, password string, c echo.Context) (bool, error) {
	c.Set("username", username)
	req := c.Request()
	req.Header.Del("Authorization")
	return true, nil
}

func (b *backendSelector) AddTarget(_ *middleware.ProxyTarget) bool {
	return false
}

func (b *backendSelector) RemoveTarget(_ string) bool {
	return false
}

func (b *backendSelector) Next(c echo.Context) *middleware.ProxyTarget {
	// Select backend based on Basic Auth username.
	username := c.Get("username").(string)
	url := url.URL{Scheme: "http", Host: b.Backends[username]}
	return &middleware.ProxyTarget{Name: username, URL: &url}
}
