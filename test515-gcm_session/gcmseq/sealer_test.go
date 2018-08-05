package gcmseq

import (
	"bytes"
	"testing"
)

func TestNewSealer_CreatesSealer(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}
	if sealer == nil {
		t.Fatalf("sealer is nil")
	}

	// Whitebox tests
	if !bytes.Equal(sealer.iv, iv) {
		t.Errorf("IV is not set: %x is set, want %x", sealer.iv, iv)
	}

	nonceSize := sealer.gcm.NonceSize()
	if len(iv) != nonceSize {
		t.Errorf("IV size %d is different from nonce size %d", len(iv), nonceSize)
	}

	if sealer.counter != 0 {
		t.Errorf("initial counter value: got %d, want 0", sealer.counter)
	}
}

func TestNewSealer_RejectsInvalidIV(t *testing.T) {
	key := []byte("0123456789abcdef")

	// Zero
	sealer, err := NewSealer(key, []byte(""))
	if err == nil {
		t.Errorf("unexpected success: sealer = %v", sealer)
	}

	// Too short 88-bit IV
	sealer, err = NewSealer(key, []byte("0123456789a"))
	if err == nil {
		t.Errorf("unexpected success: sealer = %v", sealer)
	}

	// Valid 96-bit IV
	_, err = NewSealer(key, []byte("0123456789ab"))
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	// Too long 104-bit IV
	sealer, err = NewSealer(key, []byte("0123456789abc"))
	if err == nil {
		t.Errorf("unexpected success: sealer = %v", sealer)
	}
}

func TestSealer_Seal_PrependsTagInBigEndian(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	msg := sealer.Seal([]byte("plaintext"), 0x1234)
	expected := []byte{0x12, 0x34}

	if !bytes.Equal(msg[:2], expected) {
		t.Errorf("invalid tag header: got %x, want %x", msg[:2], expected)
	}
}

func TestSealer_Seal_PrependsCounterInBigEndian(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := NewSealer(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	// counter = 1
	msg := sealer.Seal([]byte("plaintext"), 0)
	expected := []byte{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}

	if !bytes.Equal(msg[2:10], expected) {
		t.Errorf("invalid counter header: got %x, want %x", msg[2:10], expected)
	}

	// counter = 2
	msg = sealer.Seal([]byte("plaintext"), 0)
	expected = []byte{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}

	if !bytes.Equal(msg[2:10], expected) {
		t.Errorf("invalid counter header: got %x, want %x", msg[2:10], expected)
	}
}
