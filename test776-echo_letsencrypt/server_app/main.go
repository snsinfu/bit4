package main

import (
	"net/http"
	"os"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
	"github.com/pkg/errors"
	"golang.org/x/crypto/acme/autocert"
)

const certCacheDir = ".cache"

func main() {
	e := echo.New()

	e.Use(middleware.Recover())
	e.Use(middleware.Logger())

	if err := os.MkdirAll(certCacheDir, os.ModePerm); err != nil {
		e.Logger.Fatal(errors.Wrap(err, "cannot create cert cache directory"))
	}

	domain := os.Getenv("LE_DOMAIN")
	if domain == "" {
		e.Logger.Fatal("LE_DOMAIN is not set")
	}

	email := os.Getenv("LE_EMAIL")
	if email == "" {
		e.Logger.Fatal("LE_EMAIL is not set")
	}

	e.AutoTLSManager.Prompt = autocert.AcceptTOS
	e.AutoTLSManager.HostPolicy = autocert.HostWhitelist(domain)
	e.AutoTLSManager.Email = email
	e.AutoTLSManager.Cache = autocert.DirCache(certCacheDir)

	e.GET("/", func(c echo.Context) error {
		return c.String(http.StatusOK, "OK\n")
	})

	e.Logger.Fatal(e.StartAutoTLS(":443"))
}
