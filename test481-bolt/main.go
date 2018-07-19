package main

import (
	"encoding/binary"
	"log"
	"time"

	"github.com/coreos/bbolt"
	"github.com/snsinfu/go-taskch"
)

func main() {
	if err := run(); err != nil {
		log.Fatal("error:", err)
	}
}

func run() error {
	reqs := make(chan request)

	tasks := taskch.New()
	tasks.Go(func() error {
		return server(reqs)
	})

	nonce := uint64(time.Now().Unix())

	for i := 0; i < 5; i++ {
		reqs <- request{name: "alice", nonce: nonce}
		nonce++
	}
	close(reqs)

	return tasks.Wait()
}

type request struct {
	name  string
	nonce uint64
}

func server(reqs <-chan request) error {
	const bucketName = "nonce"

	db, err := bolt.Open("db.bolt", 0600, nil)
	if err != nil {
		return err
	}
	defer db.Close()

	err = db.Update(func(tx *bolt.Tx) error {
		_, err := tx.CreateBucketIfNotExists([]byte(bucketName))
		return err
	})
	if err != nil {
		return err
	}

	for req_ := range reqs {
		req := req_

		key := []byte(req.name)

		err = db.Update(func(tx *bolt.Tx) error {
			bucket := tx.Bucket([]byte(bucketName))

			if value := bucket.Get(key); value != nil {
				prevNonce := binary.BigEndian.Uint64(value)
				if req.nonce <= prevNonce {
					log.Print("Bad nonce! ", req.nonce)
					return nil
				}
			}

			log.Print("Good nonce! ", req.nonce)

			value := [8]byte{}
			binary.BigEndian.PutUint64(value[:], req.nonce)

			return bucket.Put(key, value[:])
		})
		if err != nil {
			return err
		}
	}

	return nil
}
