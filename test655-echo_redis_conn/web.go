package main

import (
	"net/http"
	"os"

	"github.com/go-redis/redis"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	opts, err := redis.ParseURL(os.Getenv("REDIS_URL"))
	if err != nil {
		e.Logger.Fatal(err)
	}

	re := redis.NewClient(opts)

	e.Use(func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			c.Set("redis", re)
			return next(c)
		}
	})

	e.GET("/", getRoot)

	e.Logger.Fatal(e.Start(":"+os.Getenv("PORT")))
}

func getRoot(c echo.Context) error {
	re := c.Get("redis").(*redis.Client)

	cnt, err := re.Incr("counter").Result()
	if err != nil {
		return err
	}

	return c.JSON(http.StatusOK, map[string]interface{}{
		"count": cnt,
	})
}
