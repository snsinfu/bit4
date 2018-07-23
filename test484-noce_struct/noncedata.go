package main

import (
	"encoding/binary"
	"errors"
)

var ErrBadInput = errors.New("bad input")

type NonceData struct {
	Nonce int64
	Data  []byte
}

func (nd NonceData) Encode() []byte {
	buf := make([]byte, 8+len(nd.Data))
	binary.BigEndian.PutUint64(buf, uint64(nd.Nonce))
	copy(buf[8:], nd.Data)
	return buf
}

func (nd *NonceData) Decode(b []byte) error {
	if len(b) < 8 {
		return ErrBadInput
	}
	nd.Nonce = int64(binary.BigEndian.Uint64(b))
	nd.Data = b[8:]
	return nil
}
