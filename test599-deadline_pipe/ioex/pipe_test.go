package ioex

import (
	"testing"
)

func TestNewPipe(t *testing.T) {
	p := NewPipe()

	// [whitebox]
	if p.data == nil {
		t.Errorf("data channel is nil")
	}

	// [whitebox]
	if p.ack == nil {
		t.Errorf("ack channel is nil")
	}
}
