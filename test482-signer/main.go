package main

import (
	"log"
	"time"

	"github.com/snsinfu/go-taskch"
)

func main() {
	signer := NewSigner([]byte("correct horse battery staple"))

	tasks := taskch.New()
	msgCh := make(chan []byte)

	// Client signs
	tasks.Go(func() error {
		data := []byte("the quick brown fox jumps over the lazy dog")
		nonce := uint64(time.Now().Unix())

		for i := 0; i < 7; i++ {
			msgCh <- signer.Sign(data, nonce)
			nonce += uint64(i % 2)
		}
		close(msgCh)

		return nil
	})

	// Server verifies
	tasks.Go(func() error {
		prevNonce := uint64(0)

		for msg := range msgCh {
			data, nonce, ok := signer.Verify(msg, prevNonce)
			if ok {
				log.Print(string(data))
				prevNonce = nonce
			} else {
				log.Print("Bad nonce!")
			}
		}

		return nil
	})

	if err := tasks.Wait(); err != nil {
		log.Fatal("error:", err)
	}
}
