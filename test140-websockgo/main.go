package main

import (
	"errors"
	"fmt"
	"log"
	"os"
	"time"

	"github.com/shopspring/decimal"
	"golang.org/x/net/websocket"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	url := os.Getenv("URL")
	channel := os.Getenv("CHANNEL")
	origin := "http://localhost/"

	ws, err := websocket.Dial(url, "", origin)
	if err != nil {
		return err
	}
	defer ws.Close()

	if err := subscribe(ws, channel); err != nil {
		return err
	}

	for {
		msg, err := receive(ws)
		if err != nil {
			return err
		}

		fmt.Printf(
			"%.3f\t%s\t%s\t%s\t%s\n",
			float64(msg.Time.UnixNano()) * 1e-9, // Safe for 100+ thousand years.
			msg.Pair,
			msg.Side,
			msg.Price,
			msg.Amount,
		)

		// HACK to prevent timestamp overlaps.
		<-time.NewTimer(1 * time.Millisecond).C
	}
}

func subscribe(ws *websocket.Conn, ch string) error {
	msg := map[string]string{
		"type":    "subscribe",
		"channel": ch,
	}
	return websocket.JSON.Send(ws, msg)
}

type point struct {
	Time   time.Time
	Serial int64
	Pair   string
	Price  decimal.Decimal
	Amount decimal.Decimal
	Side   string
}

func receive(ws *websocket.Conn) (point, error) {
	var pt point

	var msg []interface{}
	if err := websocket.JSON.Receive(ws, &msg); err != nil {
		return pt, err
	}

	serial, pair, textPrice, textAmount, side, err := unboxMsg(msg)
	if err != nil {
		log.Println("Unbox failed:", msg)
		return pt, err
	}
	price, err := decimal.NewFromString(textPrice)
	if err != nil {
		return pt, err
	}
	amount, err := decimal.NewFromString(textAmount)
	if err != nil {
		return pt, err
	}

	pt.Time = time.Now()
	pt.Serial = int64(serial)
	pt.Pair = pair
	pt.Price = price
	pt.Amount = amount
	pt.Side = side

	return pt, nil
}

func unboxMsg(msg []interface{}) (serial float64, pair, price, amount, side string, err error) {
	if len(msg) != 5 {
		err = errors.New("unexpected number of fields")
		return
	}

	serial, ok0 := msg[0].(float64)
	pair, ok1 := msg[1].(string)
	price, ok2 := msg[2].(string)
	amount, ok3 := msg[3].(string)
	side, ok4 := msg[4].(string)

	if !(ok0 && ok1 && ok2 && ok3 && ok4) {
		err = errors.New("unexpected data")
	}
	return
}
