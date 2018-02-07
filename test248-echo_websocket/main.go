package main

import (
	"os"
	"time"

	"github.com/gorilla/websocket"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

var (
	upgrader = websocket.Upgrader{}
)

func main() {
	e := echo.New()

	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	e.Static("/", "public")
	e.GET("/topics/:topic", streamTopic)

	port, ok := os.LookupEnv("PORT")
	if !ok {
		port = "3010"
	}
	e.Logger.Fatal(e.Start(":" + port))
}

// GET /topics/:topic
func streamTopic(c echo.Context) error {
	ws, err := upgrader.Upgrade(c.Response(), c.Request(), nil)
	if err != nil {
		return err
	}
	defer ws.Close()

	topic := c.Param("topic")
	message := "Topic is " + topic

	for i := 0; i < 5; i++ {
		time.Sleep(2 * time.Second)

		err := ws.WriteMessage(websocket.TextMessage, []byte(message))
		if err != nil {
			c.Logger().Error(err)
		}
	}

	return nil
}
