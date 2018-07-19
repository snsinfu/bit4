package main

import (
	"encoding/binary"
	"log"
	"time"

	"github.com/snsinfu/go-taskch"
	"github.com/syndtr/goleveldb/leveldb"
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
	db, err := leveldb.OpenFile("nonce.leveldb", nil)
	if err != nil {
		return err
	}
	defer db.Close()

	for req := range reqs {
		key := []byte(req.name)

		if value, err := db.Get(key, nil); err != leveldb.ErrNotFound {
			if err != nil {
				return err
			}

			prevNonce := binary.BigEndian.Uint64(value)
			if req.nonce <= prevNonce {
				log.Print("Bad nonce! ", req.nonce)
				continue
			}
		}

		log.Print("Good nonce! ", req.nonce)

		value := [8]byte{}
		binary.BigEndian.PutUint64(value[:], req.nonce)

		if err := db.Put(key, value[:], nil); err != nil {
			return err
		}
	}

	return nil
}
