package gcmseq

import (
	"bytes"
	"testing"
)

func TestNew_CreatesCipher(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	cipher, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}
	if cipher == nil {
		t.Fatalf("cipher is nil")
	}

	// Whitebox tests
	if !bytes.Equal(cipher.iv, iv) {
		t.Errorf("IV is not correctly set: got %x, want %x", cipher.iv, iv)
	}

	nonceSize := cipher.gcm.NonceSize()
	if len(iv) != nonceSize {
		t.Errorf("inconsistent IV size: got %d, want %d", len(iv), nonceSize)
	}

	if cipher.counter != 0 {
		t.Errorf("initial counter value: got %d, want 0", cipher.counter)
	}
}

func TestNew_RejectsInvalidIV(t *testing.T) {
	key := []byte("0123456789abcdef")

	// Zero
	cipher, err := New(key, []byte(""))
	if err == nil {
		t.Errorf("unexpected success: cipher = %v", cipher)
	}

	// Too short 88-bit IV
	cipher, err = New(key, []byte("0123456789a"))
	if err == nil {
		t.Errorf("unexpected success: cipher = %v", cipher)
	}

	// Valid 96-bit IV
	_, err = New(key, []byte("0123456789ab"))
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}

	// Too long 104-bit IV
	cipher, err = New(key, []byte("0123456789abc"))
	if err == nil {
		t.Errorf("unexpected success: cipher = %v", cipher)
	}
}

func TestCipher_Open_RejectsInvalidMessage(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	opener, err := New(key, iv)
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

func TestCipher_Open_DetectsReplayAttack(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := New(key, iv)
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

func TestCipher_Open_DetectsTagTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := New(key, iv)
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

func TestCipher_Open_DetectsCounterTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := New(key, iv)
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

func TestCipher_Open_DetectsCiphertextTampering(t *testing.T) {
	key := []byte("0123456789abcdef")
	iv := []byte("0123456789ab")

	sealer, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := New(key, iv)
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
func TestCipher_Roundtrip(t *testing.T) {
	key := []byte("0123456789abcdefghijklmnopqrstuv")
	iv := []byte("0123456789ab")

	sealer, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	opener, err := New(key, iv)
	if err != nil {
		t.Fatalf("unexpected error: %s", err)
	}

	inputs := []struct {
		tag       uint16
		plaintext []byte
	}{
		{0x1234, []byte("The quick brown fox jumps over the lazy dog")},
		{0x5678, []byte("Lorem ipsum dolor sit amet")},
		{0xa5df, []byte("Etaoin shrdlu")},
	}

	for _, input := range inputs {
		message := sealer.Seal(input.plaintext, input.tag)

		plaintext, tag, err := opener.Open(message)
		if err != nil {
			t.Fatalf("unexpected error: %s", err)
		}

		if !bytes.Equal(plaintext, input.plaintext) {
			t.Errorf("unexpected plaintext: got %q, want %q", plaintext, input.plaintext)
		}

		if tag != input.tag {
			t.Errorf("unexpected tag: got %x, want %x", tag, input.tag)
		}
	}
}

func TestCipher_Examples(t *testing.T) {
	inputs := []struct {
		tag       uint16
		plaintext []byte
		key       []byte
		iv        []byte
		message   []byte
	}{
		{
			tag:       0x1234,
			plaintext: []byte("Lorem ipsum dolor sit amet"),
			key:       []byte("764984c0596f0b0b83cc7134b01da2ce"),
			iv:        []byte("b8142da49d5f"),
			message: []byte{
				0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x73, 0xcf,
				0x3d, 0x0e, 0x1e, 0x77, 0x8e, 0xdd, 0x50, 0xf3, 0x8c, 0x94, 0x6e, 0xaf,
				0xb3, 0x97, 0x6f, 0xc4, 0xcb, 0x53, 0x52, 0x06, 0x6e, 0x94, 0x1b, 0x12,
				0x9a, 0xd6, 0x46, 0x32, 0x4e, 0x22, 0x5e, 0xf2, 0x06, 0xe3, 0xa5, 0x46,
				0xd0, 0x85, 0xb9, 0x6b,
			},
		},
		{
			tag:       0x5678,
			plaintext: []byte("The quick brown fox jumps over the lazy dog"),
			key:       []byte("f93cc9efed8278aa"),
			iv:        []byte("c4ad3e07ecd5"),
			message: []byte{
				0x56, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x5a, 0xdc,
				0xb7, 0x0a, 0x0c, 0x58, 0xe3, 0x32, 0x45, 0xab, 0x4a, 0xa4, 0x67, 0x30,
				0xb5, 0x4e, 0xed, 0xa4, 0x83, 0x2b, 0x6d, 0xd3, 0x19, 0x05, 0x17, 0x5e,
				0x9c, 0xf5, 0x1c, 0xdf, 0x01, 0x32, 0x5c, 0x75, 0x1d, 0x1c, 0xfc, 0x32,
				0x07, 0xd0, 0xc0, 0xf4, 0x1d, 0xe3, 0x5e, 0xbf, 0x23, 0x58, 0xfd, 0xb0,
				0x15, 0x92, 0xd5, 0x18, 0x89, 0xb6, 0x4e, 0xd7, 0x81,
			},
		},
	}

	// Seal
	for _, input := range inputs {
		sealer, err := New(input.key, input.iv)
		if err != nil {
			t.Fatalf("unexpected error: %s", err)
		}

		message := sealer.Seal(input.plaintext, input.tag)

		if !bytes.Equal(message, input.message) {
			t.Errorf("unexpected message: got %x, want %x", message, input.message)
		}
	}

	// Open
	for _, input := range inputs {
		opener, err := New(input.key, input.iv)
		if err != nil {
			t.Fatalf("unexpected error: %s", err)
		}

		plaintext, tag, err := opener.Open(input.message)
		if err != nil {
			t.Fatalf("unexpected error: %s", err)
		}

		if !bytes.Equal(plaintext, input.plaintext) {
			t.Errorf("unexpected plaintext: got %q, want %q", plaintext, input.plaintext)
		}

		if tag != input.tag {
			t.Errorf("unexpected plaintext: got %d, want %d", tag, input.tag)
		}
	}
}
