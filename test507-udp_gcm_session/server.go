package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/hmac"
	"crypto/rand"
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"errors"
	"io"
	"log"
	"net"
	"os"
	"time"
)

const (
	seedSize        = 16
	bufferSize      = 1472
	nonceSize       = 12
	hexMasterSecret = "ccb134bd32b6bd4b9494f72dbc0c8b07456daf666c9ef574fcbe64c5403f86c8"
)

func main() {
	if err := run(); err != nil {
		log.Fatal("error: ", err)
	}
}

func run() error {
	secret, err := hex.DecodeString(hexMasterSecret)
	if err != nil {
		return err
	}

	if len(os.Args) != 2 {
		return errors.New("specify server address")
	}

	server, err := net.ResolveUDPAddr("udp", os.Args[1])
	if err != nil {
		return err
	}

	return startServer(server, secret)
}

func startServer(server *net.UDPAddr, secret []byte) error {
	conn, err := net.ListenUDP("udp", server)
	if err != nil {
		return err
	}
	defer conn.Close()

	serverSeed := make([]byte, seedSize)
	if _, err := io.ReadFull(rand.Reader, serverSeed); err != nil {
		return err
	}

	buf := make([]byte, bufferSize)

	var client *net.UDPAddr
	var clientSeed []byte

	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if n != seedSize {
			continue
		}

		client = sender
		clientSeed = buf[:n]

		break
	}

	conn.WriteToUDP(serverSeed, client)

	// Derive keys
	mac := hmac.New(sha256.New, secret)
	mac.Write(clientSeed)
	mac.Write(serverSeed)
	clientKey := mac.Sum(nil)

	mac.Reset()
	mac.Write(serverSeed)
	mac.Write(clientSeed)
	serverKey := mac.Sum(nil)

	mac.Reset()
	mac.Write(serverKey)
	mac.Write(clientKey)
	iv := mac.Sum(nil)[:nonceSize]
	counter := uint64(time.Now().Unix()) << 16

	clientGCM, err := makeCipher(clientKey)
	if err != nil {
		return err
	}

	serverGCM, err := makeCipher(serverKey)
	if err != nil {
		return err
	}

	serverMessage := []byte("Hello!")

	counter++
	nonce := makeNonce(iv, counter)
	ciphertext := serverGCM.Seal(nil, nonce, serverMessage, nil)
	if _, err := conn.WriteToUDP(append(nonce, ciphertext...), client); err != nil {
		return err
	}

	var clientMessage []byte
	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if !equal(sender, client) {
			continue
		}

		if n < nonceSize {
			continue
		}

		nonce := buf[:nonceSize]
		ciphertext := buf[nonceSize:n]
		message, err := clientGCM.Open(nil, nonce, ciphertext, nil)
		if err != nil {
			return err
		}

		clientMessage = message
		break
	}

	log.Printf("Got client message: %q", clientMessage)

	return nil
}

func makeCipher(key []byte) (cipher.AEAD, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	return cipher.NewGCM(block)
}

func makeNonce(iv []byte, counter uint64) []byte {
	nonce := make([]byte, len(iv))
	binary.BigEndian.PutUint64(nonce, counter)
	for i := range iv {
		nonce[i] ^= iv[i]
	}
	return nonce
}

func equal(a, b *net.UDPAddr) bool {
	return a.Port == b.Port && a.IP.Equal(b.IP) && a.Zone == b.Zone
}
