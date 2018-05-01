package main

import (
	"fmt"
	"math/rand"
	"net/http"
	"os"
	"time"
	"strconv"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

type Item struct {
	Index int    `json:"index"`
	Title string `json:"title"`
}

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.File("/", "home.html")
	e.GET("/items", handleItems)

	assets := []string{
		"home.css", "normalize.css", "home.js", "zepto.min.js",
	}
	for _, file := range assets {
		e.File("/"+file, file)
	}

	port, ok := os.LookupEnv("PORT")
	if !ok {
		port = "3000"
	}
	e.Logger.Fatal(e.Start("127.0.0.1:" + port))
}

// GET /items?begin=<int>
func handleItems(c echo.Context) error {
	const (
		pageSize     = 10
		beginBits    = 16
		beginDefault = "0"
	)

	beginParam := c.QueryParam("begin")
	if beginParam == "" {
		beginParam = beginDefault
	}

	begin, err := strconv.ParseInt(beginParam, 10, beginBits)
	if err != nil {
		return c.String(http.StatusBadRequest, "")
	}

	items := make([]Item, pageSize)

	for i := 0; i < pageSize; i++ {
		items[i].Index = int(begin) + i
		items[i].Title = fmt.Sprintf("Lorem ipsum %d", items[i].Index)
	}

	// Emulate some work
	time.Sleep(1500 * time.Millisecond)

	// Emulate intermittent errors
	if rand.Intn(2) == 0 {
		return c.String(http.StatusInternalServerError, "")
	}

	return c.JSON(http.StatusOK, items)
}
