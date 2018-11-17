package main

import (
	"crypto/sha256"
	"encoding/hex"
	"log"
	"net/url"
	"os"
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

	for {
		t := time.Now()
		ts := t.Unix()
		key := makeID(ts)
		ttl := 60*time.Second

		if err := client.Set(key, ts, ttl).Err(); err != nil {
			return err
		}

		mem := redis.Z{
			Member: key,
			Score:  float64(ts),
		}
		if err := client.ZAdd("board:cards", mem).Err(); err != nil {
			return err
		}

		log.Print("Added ", key)

		time.Sleep(time.Second)
	}
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

func makeID(n int64) string {
	hash := sha256.Sum256([]byte{
		byte(n),
		byte(n >> 8),
		byte(n >> 16),
		byte(n >> 24),
		byte(n >> 32),
		byte(n >> 40),
		byte(n >> 48),
		byte(n >> 56),
	})
	return hex.EncodeToString(hash[:4])
}
