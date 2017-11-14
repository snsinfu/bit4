package main

import (
	"log"

	"github.com/emersion/go-imap"
	"github.com/emersion/go-imap/client"
)

func main() {
	const server = "localhost:10143"
	const username = "ubuntu"
	const password = "ubuntu"

	cli, err := client.Dial(server)
	if err != nil {
		log.Fatal(err)
	}
	defer cli.Logout()

	// go-imap nor go-sasl do not support CRAM-MD5. So just fall back to
	// plaintext authentication here. In production, we should be using TLS.
	if err := cli.Login(username, password); err != nil {
		log.Fatal(err)
	}

	// Have no idea why go-imap made List command asynchronous while other
	// commands such as Select are synchronous...
	mailboxes := make(chan *imap.MailboxInfo, 10)
	done := make(chan error, 1)
	go func() {
		done <- cli.List("", "*", mailboxes)
	}()

	for mbox := range mailboxes {
		log.Println(mbox)
	}

	if err := <-done; err != nil {
		log.Fatal(err)
	}

	// We only have INBOX in the test environment
	mbox, err := cli.Select("INBOX", false)
	if err != nil {
		log.Fatal(err)
	}
	log.Println(mbox.Flags)
}
