package main

import (
	"crypto/sha256"
	"encoding/hex"
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

	for {
		t := time.Now()
		ts := t.Unix()
		key := makeID(ts)

		if _, err := conn.Do("SET", key, ts); err != nil {
			return err
		}

		if _, err := conn.Do("ZADD", "board:cards", ts, key); err != nil {
			return err
		}

		log.Print("Added ", key)

		time.Sleep(time.Second)
	}
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
