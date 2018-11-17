package main

import (
	"log"
	"os"
	"time"

	"github.com/gomodule/redigo/redis"
)

func main() {
	if err := run(); err != nil {
		log.Fatal("error: ", err)
	}
}

func run() error {
	conn, err := redis.DialURL(os.Getenv("REDIS_URL"))
	if err != nil {
		return err
	}
	defer conn.Close()

	const (
		checkInterval = 5 * time.Second
		staleAge      = 10 * time.Second
	)

	for now := range time.Tick(checkInterval) {
		t := now.Add(-staleAge)

		rs, err := redis.Values(conn.Do("ZRANGEBYSCORE", "board:cards", "-inf", t.Unix()))
		if err != nil {
			return err
		}

		if len(rs) == 0 {
			continue
		}

		args := append([]interface{}{"board:cards"}, rs...)
		r, err := redis.Int(conn.Do("ZREM", args...))
		if err != nil {
			return err
		}

		log.Printf("Removed %d entries", r)
	}

	return nil
}
