package scanner

import (
	"testing"
	"text/scanner"
)

func Test_Scanner_SetsErrorHandler(t *testing.T) {
	sc := New("")

	if sc.scanner.Error == nil {
		t.Error("scanner should set error handler")
	}
}

func Test_Scanner_DoesNotSkipComments(t *testing.T) {
	sc := New("")

	if sc.scanner.Mode&scanner.SkipComments != 0 {
		t.Error("scanner should not skip comments")
	}
}

func Test_Scanner_Err_InitiallyReturnsNil(t *testing.T) {
	sc := New("")

	if err := sc.Err(); err != nil {
		t.Errorf("unexpected error: %s", err)
	}
}

func Test_Scanner_ScanIdent_AcceptsValidInput(t *testing.T) {
	tokens := []string{
		"_",
		"_1",
		"ident",
	}

	for _, expected := range tokens {
		sc := New(expected)

		actual := sc.ScanIdent()
		if actual != expected {
			t.Errorf("unexpected token: %s, want %s", actual, expected)
		}

		err := sc.Err()
		if err != nil {
			t.Errorf("unexpected error: %s", err)
		}
	}
}

func Test_Scanner_ScanIdent_DetectsError(t *testing.T) {
	sources := []string{
		"",
		" ",
		"::",
		"123",
	}

	for _, src := range sources {
		sc := New(src)

		sc.ScanIdent()
		if sc.Err() == nil {
			t.Errorf("ScanIdent unexpectedly succeeds for '%s'", src)
		}
	}
}

func Test_Scanner_ScanInt_AcceptsValidInput(t *testing.T) {
	examples := []struct {
		source   string
		expected int
	}{
		{"0", 0},
		{"1", 1},
		{"123", 123},
		{"000123", 123},
	}

	for _, example := range examples {
		sc := New(example.source)

		actual := sc.ScanInt()
		if actual != example.expected {
			t.Errorf("unexpected integer: %d, want %d", actual, example.expected)
		}

		err := sc.Err()
		if err != nil {
			t.Errorf("unexpected error: %s", err)
		}
	}
}

func Test_Scanner_ScanInt_DetectsError(t *testing.T) {
	sources := []string{
		"",
		" ",
		"::",
		"-1",     // negative
		"0xabcd", // not decimal
		"identifier",
	}

	for _, src := range sources {
		sc := New(src)

		sc.ScanInt()
		if sc.Err() == nil {
			t.Errorf("ScanInt unexpectedly succeeds for '%s'", src)
		}
	}
}

func Test_Scanner_Consume_AcceptsValidInput(t *testing.T) {
	examples := []struct {
		source   string
		expected rune
	}{
		{":", ':'},
		{"::", ':'},
		{"!?", '!'},
	}

	for _, example := range examples {
		sc := New(example.source)

		actual := sc.Consume(example.expected)
		if actual != example.expected {
			t.Errorf("unexpected rune: %c, want %c", actual, example.expected)
		}

		err := sc.Err()
		if err != nil {
			t.Errorf("unexpected error: %s", err)
		}
	}
}

func Test_Scanner_Consume_DetectsError(t *testing.T) {
	examples := []struct{
		source   string
		expected rune
	}{
		{"", ':'},
		{" ", ':'},
		{"/", ':'},
		{":", '/'},
		{"100", '/'},
		{"identifier", '/'},
	}

	for _, example := range examples {
		sc := New(example.source)

		sc.Consume(example.expected)
		if sc.Err() == nil {
			t.Errorf("Consume(%c) unexpectedly succeeds for '%s'", example.expected, example.source)
		}
	}
}

func Test_Scanner_ScansMultipleTokens(t *testing.T) {
	sc := New("22/tcp")

	port := sc.ScanInt()
	delim := sc.Consume('/')
	proto := sc.ScanIdent()

	if port != 22 {
		t.Errorf("unexpected token: %d, want %d", port, 22)
	}

	if delim != '/' {
		t.Errorf("unexpected token: %c, want %c", delim, '/')
	}

	if proto != "tcp" {
		t.Errorf("unexpected token: %s, want %s", proto, "tcp")
	}
}

func Test_Scanner_SkipsWhitespaces(t *testing.T) {
	sc := New("  foo  bar  ")

	foo := sc.ScanIdent()
	bar := sc.ScanIdent()

	if foo != "foo" {
		t.Errorf("unexpected token: %s, want %s", foo, "foo")
	}

	if bar != "bar" {
		t.Errorf("unexpected token: %s, want %s", bar, "bar")
	}
}

func Test_Scanner_Stop_ReturnsNilOnSuccess(t *testing.T) {
	sc := New("22/tcp")

	sc.ScanInt()
	sc.Consume('/')
	sc.ScanIdent()

	err := sc.Stop()
	if err != nil {
		t.Errorf("unexpected error: %s", err)
	}
}

func Test_Scanner_Stop_FailsIfSourceIsNotEmpty(t *testing.T) {
	sc := New("22:")

	sc.ScanInt()

	if sc.Stop() == nil {
		t.Error("Stop unexpectedly succeeds")
	}
}

func Test_Scanner_Stop_ReturnsFirstError(t *testing.T) {
	sc := New("22/tcp")

	sc.ScanIdent()
	expected := sc.Err()
	sc.Consume(':')
	sc.ScanInt()

	actual := sc.Stop()
	if actual != expected {
		t.Errorf("unexpected error: '%s', want '%s'", actual, expected)
	}
}
