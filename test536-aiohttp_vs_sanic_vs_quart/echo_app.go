package main

import (
	"log"
	"net/http"
	"os"
	"runtime/pprof"
	"strconv"
	"time"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	f, err := os.Create("echo.prof")
	if err != nil {
		log.Fatal(err)
	}

	if err := pprof.StartCPUProfile(f); err != nil {
		log.Fatal(err)
	}

	go func() {
		<-time.After(10*time.Second)
		pprof.StopCPUProfile()
		f.Close()
	}()

	e := echo.New()

	e.Use(middleware.Recover())
	e.Use(middleware.Logger())

	e.GET("/resource/:resid", resource)

	e.Logger.Fatal(e.Start("127.0.0.1:8080"))
}

func resource(c echo.Context) error {
	resid, err := strconv.Atoi(c.Param("resid"))
	if err != nil {
		return err
	}
	return c.JSON(http.StatusOK, map[string]interface{}{
		"client": c.Request().RemoteAddr,
		"id":     resid,
	})
}
