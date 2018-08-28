package main

import (
	"crypto/aes"
	"crypto/cipher"
	"encoding/binary"
	"errors"
	"fmt"
	"os"

	"github.com/vmihailenco/msgpack"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	codec := NewRawCodec()
	msg, _ := codec.Encode(0x12, []byte("Hello"))
	tag, body, _ := codec.Decode(msg)

	fmt.Printf("Message: %x\n", msg)
	fmt.Printf("Content: x%02x | %q\n", tag, body)
	fmt.Println()

	codec, _ = NewSecureCodec([]byte("0123456789abcdef"), []byte("opqrstuvwxyz"))
	msg, _ = codec.Encode(0x12, []byte("Hello"))
	tag, body, _ = codec.Decode(msg)

	fmt.Printf("Message: %x\n", msg)
	fmt.Printf("Content: x%02x | %q\n", tag, body)

	return nil
}

type Codec interface {
	Encode(tag byte, data interface{}) ([]byte, error)
	Decode(msg []byte) (byte, []byte, error)
}

type RawCodec struct {
}

func NewRawCodec() Codec {
	return &RawCodec{}
}

func (c *RawCodec) Encode(tag byte, data interface{}) ([]byte, error) {
	var body []byte

	switch data.(type) {
	case nil:

	case []byte:
		body = data.([]byte)

	default:
		var err error
		body, err = msgpack.Marshal(data)
		if err != nil {
			return nil, err
		}
	}

	return append([]byte{tag}, body...), nil
}

func (c *RawCodec) Decode(msg []byte) (byte, []byte, error) {
	if len(msg) == 0 {
		return 0, nil, errors.New("invalid message")
	}
	return msg[0], msg[1:], nil
}

type SecureCodec struct {
	gcm     cipher.AEAD
	iv      []byte
	counter uint64
}

func NewSecureCodec(key, iv []byte) (Codec, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	gcm, err := cipher.NewGCM(block)
	if err != nil {
		return nil, err
	}

	codec := &SecureCodec{
		gcm: gcm,
		iv:  make([]byte, len(iv)),
	}
	copy(codec.iv, iv)

	return codec, nil
}

func (c *SecureCodec) Encode(tag byte, data interface{}) ([]byte, error) {
	var body []byte

	switch data.(type) {
	case nil:

	case []byte:
		body = data.([]byte)

	default:
		var err error
		body, err = msgpack.Marshal(data)
		if err != nil {
			return nil, err
		}
	}

	c.counter++

	var counterCode [8]byte
	binary.LittleEndian.PutUint64(counterCode[:], c.counter)

	msg := make([]byte, 1+8+16+len(body))

	msg[0] = tag
	copy(msg[1:], counterCode[:])

	var nonce [12]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	c.gcm.Seal(msg[:9], nonce[:], body, msg[:9])

	return msg, nil
}

func (c *SecureCodec) Decode(msg []byte) (byte, []byte, error) {
	tag := msg[0]
	counterCode := msg[1:9]

	var nonce [12]byte
	copy(nonce[:], c.iv)
	for i := range counterCode {
		nonce[i] ^= counterCode[i]
	}

	body, err := c.gcm.Open(nil, nonce[:], msg[9:], msg[:9])
	if err != nil {
		return 0, nil, err
	}

	return tag, body, nil
}
