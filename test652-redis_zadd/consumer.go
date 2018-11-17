package main

import (
	"log"
	"net/url"
	"os"
	"strconv"
	"time"

	"github.com/go-redis/redis"
)

func main() {
	if err := run(); err != nil {
		log.Fatal("error: ", err)
	}
}

func run() error {
	client, err := newClientURL(os.Getenv("REDIS_URL"))
	if err != nil {
		return err
	}
	defer client.Close()

	const (
		checkInterval = 5 * time.Second
		staleAge      = 10 * time.Second
	)

	for now := range time.Tick(checkInterval) {
		t := now.Add(-staleAge)

		members, err := client.ZRangeByScore("board:cards", redis.ZRangeBy{
			Min: "-inf",
			Max: strconv.FormatInt(t.Unix(), 10),
		}).Result()
		if err != nil {
			return err
		}

		if len(members) == 0 {
			continue
		}

		membersArg := []interface{}{}
		for _, m := range members {
			membersArg = append(membersArg, m)
		}

		n, err := client.ZRem("board:cards", membersArg...).Result()
		if err != nil {
			return err
		}

		log.Printf("Removed %d entries", n)
	}

	return nil
}

func newClientURL(u string) (*redis.Client, error) {
	ru, err := url.Parse(u)
	if err != nil {
		return nil, err
	}

	pass, _ := ru.User.Password()

	client := redis.NewClient(&redis.Options{
		Addr:     ru.Host,
		Password: pass,
	})

	return client, nil
}
