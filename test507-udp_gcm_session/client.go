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

	return startClient(server, secret)
}

func startClient(server *net.UDPAddr, secret []byte) error {
	conn, err := net.ListenUDP("udp", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	// Exchange session seed
	clientSeed := make([]byte, seedSize)
	if _, err := io.ReadFull(rand.Reader, clientSeed); err != nil {
		return err
	}

	if _, err := conn.WriteToUDP(clientSeed, server); err != nil {
		return err
	}

	var serverSeed []byte

	buf := make([]byte, bufferSize)
	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if !equal(sender, server) {
			continue
		}

		if n != seedSize {
			return errors.New("unrecognized message")
		}

		serverSeed = buf[:n]
		break
	}

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
	mac.Write(clientKey)
	mac.Write(serverKey)
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

	var serverMessage []byte
	for {
		n, sender, err := conn.ReadFromUDP(buf)
		if err != nil {
			return err
		}

		if !equal(sender, server) {
			continue
		}

		if n < nonceSize {
			continue
		}

		nonce := buf[:nonceSize]
		ciphertext := buf[nonceSize:n]
		message, err := serverGCM.Open(nil, nonce, ciphertext, nil)
		if err != nil {
			return err
		}

		serverMessage = message
		break
	}

	log.Printf("Got server message: %q", serverMessage)

	clientMessage := append([]byte("OK: "), serverMessage...)

	counter++
	nonce := makeNonce(iv, counter)
	ciphertext := clientGCM.Seal(nil, nonce, clientMessage, nil)
	if _, err := conn.WriteToUDP(append(nonce, ciphertext...), server); err != nil {
		return err
	}

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
