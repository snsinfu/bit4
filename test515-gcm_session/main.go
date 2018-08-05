package main

import (
	"fmt"
	"os"

	"github.com/snsinfu/bit4/test515-gcm_session/gcmseq"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	key := []byte("eb17ca6fd73bceb8a22306d70dd48fe3")
	iv := []byte("b72acff833c9")

	sealer, err := gcmseq.NewSealer(key, iv)
	if err != nil {
		return err
	}

	opener, err := gcmseq.NewOpener(key, iv)
	if err != nil {
		return err
	}

	for i := 0; i < 5; i++ {
		msg := sealer.Seal([]byte("Hello!"), 0x1234)

		// Tamper with the message
		switch i {
		case 1:
			msg[0] = 0x56
			msg[1] = 0x78
		case 3:
			msg[9] = 1
		}

		fmt.Printf("message: (%d) %x\n", len(msg), msg)

		plaintext, tag, err := opener.Open(msg)
		if err != nil {
			fmt.Printf("error: %s\n\n", err)
			continue
		}

		fmt.Printf("tag: 0x%x\n", tag)
		fmt.Printf("plaintext: (%d) %q\n", len(plaintext), plaintext)
		fmt.Println()
	}

	return nil
}
