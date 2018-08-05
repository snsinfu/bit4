package gcmseq

import (
	"bytes"
	"testing"
)

func TestNewOpener_CreatesOpener(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}
	if opener == nil {
		t.Fatalf("opener is nil")
	}

	// Whitebox tests
	if !bytes.Equal(opener.iv, iv) {
		t.Errorf("IV is not set: %x is set, want %x", opener.iv, iv)
	}

	nonceSize := opener.gcm.NonceSize()
	if len(iv) != nonceSize {
		t.Errorf("IV size %d is different from nonce size %d", len(iv), nonceSize)
	}

	if opener.counter != 0 {
		t.Errorf("initial counter value: got %d, want 0", opener.counter)
	}
}

func TestNewOpener_RejectsInvalidIV(t *testing.T) {
	key := []byte("0123456789abcdef")

	// Zero
	_, err := NewOpener(key, []byte(""))
	if err != ErrInvalidIV {
		t.Errorf("unexpected condition: err = %v", err)
	}

	// Too short 88-bit IV
	_, err = NewOpener(key, []byte("0123456789a"))
	if err != ErrInvalidIV {
		t.Errorf("unexpected condition: err = %v", err)
	}

	// Valid 96-bit IV
	_, err = NewOpener(key, []byte("0123456789ab"))
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	// Too long 104-bit IV
	_, err = NewOpener(key, []byte("0123456789abc"))
	if err != ErrInvalidIV {
		t.Errorf("unexpected condition: err = %v", err)
	}
}

func TestOpener_Open_RejectsInvalidMessage(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	// Empty
	_, _, err = opener.Open([]byte(""))
	if err != ErrInvalidMessage {
		t.Errorf("unexpected condition: err = %v", err)
	}

	// Short
	_, _, err = opener.Open([]byte{0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01})
	if err != ErrInvalidMessage {
		t.Errorf("unexpected condition: err = %v", err)
	}
}

func TestOpener_Open_DetectsReplayAttack(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	msg1 := sealer.Seal([]byte("Hello"), 1)
	msg2 := sealer.Seal([]byte("See you"), 2)

	// First message
	_, _, err = opener.Open(msg1)
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	// Replay
	_, _, err = opener.Open(msg1)
	if err != ErrReplayAttack {
		t.Errorf("unexpected condition: err = %v", err)
	}

	// Second message
	_, _, err = opener.Open(msg2)
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	// Replay
	_, _, err = opener.Open(msg2)
	if err != ErrReplayAttack {
		t.Errorf("unexpected condition: err = %v", err)
	}

	// Replay
	_, _, err = opener.Open(msg1)
	if err != ErrReplayAttack {
		t.Errorf("unexpected condition: err = %v", err)
	}
}

func TestOpener_Open_DetectsTagTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	// Tamper with a tag value [0..1]
	msg := sealer.Seal([]byte("Hello"), 0x1234)
	msg[0] = 0x56
	msg[1] = 0x78

	_, _, err = opener.Open(msg)
	if err == nil {
		t.Error("unexpected success")
	}
}

func TestOpener_Open_DetectsCounterTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	// Tamper with a counter value [2..9]
	msg := sealer.Seal([]byte("Hello"), 0x1234)
	msg[9] = 2

	_, _, err = opener.Open(msg)
	if err == nil {
		t.Error("unexpected success")
	}
}

func TestOpener_Open_DetectsCiphertextTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := NewOpener(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	// Tamper with a ciphertext
	msg := sealer.Seal([]byte("Hello"), 0x1234)
	msg[10] ^= 0xcc

	_, _, err = opener.Open(msg)
	if err == nil {
		t.Error("unexpected success")
	}
}
