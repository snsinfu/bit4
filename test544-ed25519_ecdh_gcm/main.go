package main

import (
	"bytes"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"io"
	"os"

	"golang.org/x/crypto/curve25519"
	"golang.org/x/crypto/ed25519"
	"golang.org/x/crypto/hkdf"

	"github.com/snsinfu/go-taskch"
	"github.com/vmihailenco/msgpack"
)

// Ed25519 + ECDH + GCM secure channel
//
// - Ed25519 for server verification
// - ECDH for key exchange
// - GCM for encryption and message authentication
// - AES-128 with counter-modulated nonce
// - MessagePack serialization

func main() {
	tasks := taskch.New()

	serverReader, serverWriter := io.Pipe()
	clientReader, clientWriter := io.Pipe()

	tasks.Go(func() error {
		return server(clientReader, serverWriter)
	})

	tasks.Go(func() error {
		return client(serverReader, clientWriter)
	})

	if err := tasks.Wait(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

var (
	serverSeed   = mustDecodeHex("b026cf31fcd92e324ea6c397a40c126e619038e4ce246de81baf00316ccd8e80")
	serverPubkey = mustDecodeHex("f1f921d1b0efb7abe62131d528cfdac93ed6c2b851b19cfdae09761364944962")
)

// C -> S:  ClientHello | { random, ecdh_pubkey }
// S -> C:  ServerHello | { random, ecdh_pubkey, signature }
// C -> S:  <GCM> tyMessage | counter | { message }
// S -> C:  <GCM> tyAck | counter

const (
	tyClientHello byte = iota
	tyServerHello
	tyMessage
	tyAck
)

type msgpackClientHello struct {
	Random     []byte `msgpack:"random"`
	ECDHPubkey []byte `msgpack:"ecdh_pubkey"`
}

type msgpackServerHello struct {
	Random     []byte `msgpack:"random"`
	ECDHPubkey []byte `msgpack:"ecdh_pubkey"`
	Signature  []byte `msgpack:"signature"`
}

type msgpackMessage struct {
	Message string `msgpack:"message"`
}

func server(r io.Reader, w io.Writer) error {
	buf := make([]byte, 512)

	// C -> S:  ClientHello | { random, ecdh_pubkey }
	var clientHello msgpackClientHello

	for {
		n, err := r.Read(buf)
		if err != nil {
			return err
		}

		if buf[0] != tyClientHello {
			return fmt.Errorf("unexpected message: got %x, want tyClientHello", buf[0])
		}

		if err := msgpack.Unmarshal(buf[1:n], &clientHello); err != nil {
			return err
		}

		break
	}

	// Server-side session random
	serverRandom := make([]byte, 16)
	if _, err := io.ReadFull(rand.Reader, serverRandom); err != nil {
		return err
	}

	// Server-side ECDH keys
	var ecdhServerPriv [32]byte
	var ecdhServerPub [32]byte

	if _, err := io.ReadFull(rand.Reader, ecdhServerPriv[:]); err != nil {
		return err
	}

	curve25519.ScalarBaseMult(&ecdhServerPub, &ecdhServerPriv)

	// Generate signature
	serverPrivkey := ed25519.NewKeyFromSeed(serverSeed)

	signedData := bytes.NewBuffer(nil)
	signedData.Write(serverRandom)
	signedData.Write(clientHello.Random)

	signature := ed25519.Sign(serverPrivkey, signedData.Bytes())

	// S -> C:  ServerHello | { random, ecdh_pubkey, signature }
	serverHello := msgpackServerHello{
		Random:     serverRandom,
		ECDHPubkey: ecdhServerPub[:],
		Signature:  signature,
	}

	payload, err := msgpack.Marshal(serverHello)
	if err != nil {
		return err
	}

	buf[0] = tyServerHello
	n := copy(buf[1:], payload) + 1

	if _, err := w.Write(buf[:n]); err != nil {
		return err
	}

	// Derive shared secret
	var ecdhSecret [32]byte
	var ecdhClientPub [32]byte

	copy(ecdhClientPub[:], clientHello.ECDHPubkey)
	curve25519.ScalarMult(&ecdhSecret, &ecdhServerPriv, &ecdhClientPub)

	// Derive session keys
	salt := append(serverHello.Random, clientHello.Random...)
	keys := hkdf.New(sha256.New, ecdhSecret[:], salt, nil)

	serverKey := make([]byte, 16)
	clientKey := make([]byte, 16)
	serverIV := make([]byte, 12)
	clientIV := make([]byte, 12)

	if _, err := io.ReadFull(keys, serverKey); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, clientKey); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, serverIV); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, clientIV); err != nil {
		return err
	}

	fmt.Printf("[server] server key: %x\n", serverKey)
	fmt.Printf("[server] client key: %x\n", clientKey)
	fmt.Printf("[server] server IV: %x\n", serverIV)
	fmt.Printf("[server] client IV: %x\n", clientIV)

	// Make cipher
	serverBlock, err := aes.NewCipher(serverKey)
	if err != nil {
		return err
	}

	serverGCM, err := cipher.NewGCM(serverBlock)
	if err != nil {
		return err
	}

	clientBlock, err := aes.NewCipher(clientKey)
	if err != nil {
		return err
	}

	clientGCM, err := cipher.NewGCM(clientBlock)
	if err != nil {
		return err
	}

	serverCounter := uint64(0)
	clientCounter := uint64(0)

	// C -> S:  <GCM> Message | counter | { message }
	n, err = r.Read(buf)
	if err != nil {
		return err
	}

	if buf[0] != tyMessage {
		return fmt.Errorf("unexpected message: got %x, want tyMessage", buf[0])
	}

	counter := binary.LittleEndian.Uint64(buf[1:])
	if counter <= clientCounter {
		return fmt.Errorf("nonce error")
	}
	clientCounter = counter

	counterBytes := make([]byte, 8)
	nonce := make([]byte, 12)
	copy(counterBytes, buf[1:])
	copy(nonce, clientIV)
	for i := 0; i < len(counterBytes); i++ {
		nonce[i] ^= counterBytes[i]
	}

	payload, err = clientGCM.Open(nil, nonce, buf[9:n], buf[:9])
	if err != nil {
		return err
	}

	var message msgpackMessage

	if err := msgpack.Unmarshal(payload, &message); err != nil {
		return err
	}

	fmt.Printf("[server] Received encrypted packet %q\n", buf[:n])
	fmt.Printf("[server] Got message %q\n", message.Message)

	// S -> C:  <GCM> Ack | counter
	serverCounter++
	binary.LittleEndian.PutUint64(counterBytes, serverCounter)

	copy(nonce, serverIV)
	for i := 0; i < len(counterBytes); i++ {
		nonce[i] ^= counterBytes[i]
	}

	n = 1
	buf[0] = tyAck
	n += copy(buf[n:], counterBytes)

	msg := serverGCM.Seal(buf[:n], nonce, nil, buf[:n])
	if _, err := w.Write(msg); err != nil {
		return err
	}

	fmt.Println("[server] Done")

	return nil
}

func client(r io.Reader, w io.Writer) error {
	buf := make([]byte, 512)

	// Client-side session random
	clientRandom := make([]byte, 16)
	if _, err := io.ReadFull(rand.Reader, clientRandom); err != nil {
		return err
	}

	// Client-side ECDH keys
	var ecdhClientPriv [32]byte
	var ecdhClientPub [32]byte

	if _, err := io.ReadFull(rand.Reader, ecdhClientPriv[:]); err != nil {
		return err
	}

	curve25519.ScalarBaseMult(&ecdhClientPub, &ecdhClientPriv)

	// C -> S:  ClientHello | { random, ecdh_pubkey }
	clientHello := msgpackClientHello{
		Random:     clientRandom,
		ECDHPubkey: ecdhClientPub[:],
	}

	payload, err := msgpack.Marshal(clientHello)
	if err != nil {
		return err
	}

	buf[0] = tyClientHello
	n := copy(buf[1:], payload) + 1

	if _, err := w.Write(buf[:n]); err != nil {
		return err
	}

	// S -> C:  ServerHello | { random, ecdh_pubkey, signature }
	var serverHello msgpackServerHello

	for {
		n, err := r.Read(buf)
		if err != nil {
			return err
		}

		if buf[0] != tyServerHello {
			return fmt.Errorf("unexpected message: got %x, want tyServerHello", buf[0])
		}

		if err := msgpack.Unmarshal(buf[1:n], &serverHello); err != nil {
			return err
		}

		break
	}

	// Verify signature
	signedData := bytes.NewBuffer(nil)
	signedData.Write(serverHello.Random)
	signedData.Write(clientRandom)

	if !ed25519.Verify(ed25519.PublicKey(serverPubkey), signedData.Bytes(), serverHello.Signature) {
		return fmt.Errorf("bad signature")
	}

	// Derive shared secret
	var ecdhSecret [32]byte
	var ecdhServerPub [32]byte

	copy(ecdhServerPub[:], serverHello.ECDHPubkey)
	curve25519.ScalarMult(&ecdhSecret, &ecdhClientPriv, &ecdhServerPub)

	// Derive session keys
	salt := append(serverHello.Random, clientHello.Random...)
	keys := hkdf.New(sha256.New, ecdhSecret[:], salt, nil)

	serverKey := make([]byte, 16)
	clientKey := make([]byte, 16)
	serverIV := make([]byte, 12)
	clientIV := make([]byte, 12)

	if _, err := io.ReadFull(keys, serverKey); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, clientKey); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, serverIV); err != nil {
		return err
	}

	if _, err := io.ReadFull(keys, clientIV); err != nil {
		return err
	}

	fmt.Printf("[client] server key: %x\n", serverKey)
	fmt.Printf("[client] client key: %x\n", clientKey)
	fmt.Printf("[client] server IV: %x\n", serverIV)
	fmt.Printf("[client] client IV: %x\n", clientIV)

	// Make cipher
	serverBlock, err := aes.NewCipher(serverKey)
	if err != nil {
		return err
	}

	serverGCM, err := cipher.NewGCM(serverBlock)
	if err != nil {
		return err
	}

	clientBlock, err := aes.NewCipher(clientKey)
	if err != nil {
		return err
	}

	clientGCM, err := cipher.NewGCM(clientBlock)
	if err != nil {
		return err
	}

	serverCounter := uint64(0)
	clientCounter := uint64(0)

	// C -> S:  <GCM> Message | counter | { message }
	message := msgpackMessage{
		Message: "Lorem ipsum dolor sit amet",
	}

	payload, err = msgpack.Marshal(message)
	if err != nil {
		return err
	}

	counterBytes := make([]byte, 8)
	nonce := make([]byte, 12)

	clientCounter++
	binary.LittleEndian.PutUint64(counterBytes, clientCounter)

	copy(nonce, clientIV)
	for i := 0; i < len(counterBytes); i++ {
		nonce[i] ^= counterBytes[i]
	}

	n = 1
	buf[0] = tyMessage
	n += copy(buf[n:], counterBytes)

	msg := clientGCM.Seal(buf[:n], nonce, payload, buf[:n])
	if _, err := w.Write(msg); err != nil {
		return err
	}

	// S -> C:  <GCM> Ack | counter
	n, err = r.Read(buf)
	if err != nil {
		return err
	}

	if buf[0] != tyAck {
		return fmt.Errorf("unexpected message: got %x, want tyAck", buf[0])
	}

	counter := binary.LittleEndian.Uint64(buf[1:])
	if counter <= serverCounter {
		return fmt.Errorf("nonce error")
	}
	serverCounter = counter

	copy(counterBytes, buf[1:])
	copy(nonce, serverIV)
	for i := 0; i < len(counterBytes); i++ {
		nonce[i] ^= counterBytes[i]
	}

	payload, err = serverGCM.Open(nil, nonce, buf[9:n], buf[:9])
	if err != nil {
		return err
	}

	if len(payload) != 0 {
		return fmt.Errorf("invalid ack payload")
	}

	fmt.Println("[client] Got Ack")
	fmt.Println("[client] Done")

	return nil
}

func mustDecodeHex(s string) []byte {
	data, err := hex.DecodeString(s)
	if err != nil {
		panic(err)
	}
	return data
}
