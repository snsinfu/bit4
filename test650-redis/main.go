package main

import (
	"fmt"
	"log"
	"net/url"
	"os"
	"strings"
	"time"

	"github.com/go-redis/redis"
	"github.com/snsinfu/go-taskch"
)

func main() {
	tasks := taskch.New()

	tasks.Go(produce)
	tasks.Go(consume)

	if err := tasks.Wait(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func produce() error {
	client, err := connect(os.Getenv("REDIS_URL"))
	if err != nil {
		return err
	}
	defer client.Close()

	if err := client.ConfigSet("notify-keyspace-events", "Kx").Err(); err != nil {
		return err
	}

	for i := 0; i < 10; i++ {
		key := fmt.Sprintf("test:key.%d", i)
		val := i
		ttl := 5 * time.Second

		if err := client.Set(key, val, ttl).Err(); err != nil {
			return err
		}

		time.Sleep(time.Second)
	}

	return nil
}

func consume() error {
	client, err := connect(os.Getenv("REDIS_URL"))
	if err != nil {
		return err
	}
	defer client.Close()

	pubsub := client.PSubscribe("__keyspace@0__:test:key.*")
	defer pubsub.Close()

	for msg := range pubsub.Channel() {
		key := strings.TrimPrefix(msg.Channel, "__keyspace@0__:")
		log.Printf("Key %q expired", key)
	}

	return nil
}

func connect(u string) (*redis.Client, error) {
	re, err := url.Parse(u)
	if err != nil {
		return nil, err
	}

	pass, ok := re.User.Password()
	if !ok {
		pass = ""
	}

	client := redis.NewClient(&redis.Options{
		Addr:     re.Host,
		Password: pass,
	})
	return client, nil
}
