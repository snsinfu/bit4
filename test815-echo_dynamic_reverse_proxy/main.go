package main

import (
	"net/url"
	"strings"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.Use(middleware.Proxy(newDynamicBalancer()))

	e.Logger.Fatal(e.Start("localhost:4000"))
	panic("unreachable")
}

type dynamicBalancer struct {
	backends map[string]*middleware.ProxyTarget
}

func newDynamicBalancer() *dynamicBalancer {
	return &dynamicBalancer{
		backends: map[string]*middleware.ProxyTarget{},
	}
}

func (b *dynamicBalancer) AddTarget(_ *middleware.ProxyTarget) bool {
	return false
}

func (b *dynamicBalancer) RemoveTarget(_ string) bool {
	return false
}

func (b *dynamicBalancer) Next(c echo.Context) *middleware.ProxyTarget {
	path := strings.TrimPrefix(c.Path(), "/")
	name, subpath := splitOnce(path, "/")
	c.SetPath(subpath)

	backend, ok := b.backends[name]
	if !ok {
		// This backend selection can be dynamic. Querying a database for the
		// appropriate backend based on the name, for example.
		host := name + ".localhost:9000"

		url := &url.URL{Scheme: "http", Host: host}
		backend = &middleware.ProxyTarget{Name: name, URL: url}
		b.backends[name] = backend
	}

	return backend
}

func splitOnce(s, delim string) (string, string) {
	pos := strings.Index(s, delim)
	return s[:pos], s[pos:]
}
