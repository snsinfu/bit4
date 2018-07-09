package main

import (
	"errors"
	"strconv"
	"strings"
	"text/scanner"
)

type Scanner struct {
	scanner scanner.Scanner
	err     error
}

func New(s string) *Scanner {
	sc := &Scanner{}
	sc.scanner.Init(strings.NewReader(s))
	return sc
}

func (sc *Scanner) Err() error {
	return sc.err
}

func (sc *Scanner) scan(mode uint) rune {
	sc.scanner.Mode = mode
	return sc.scanner.Scan()
}

func (sc *Scanner) Stop() error {
	if sc.err == nil && sc.scan(0) != scanner.EOF {
		sc.err = errors.New("remaining token at the end of input")
	}
	return sc.err
}

func (sc *Scanner) ScanString() string {
	if sc.err != nil {
		return ""
	}

	tok := sc.scan(scanner.ScanIdents)
	if tok == scanner.EOF {
		sc.err = errors.New("unexpected EOF")
	}

	return sc.scanner.TokenText()
}

func (sc *Scanner) ScanRune() rune {
	if sc.err != nil {
		return 0
	}

	ch := sc.scanner.Next()
	if ch == scanner.EOF {
		sc.err = errors.New("unexpected EOF")
	}

	return ch
}

func (sc *Scanner) ScanInt() int {
	if sc.err != nil {
		return 0
	}

	negate := false
	tok := sc.scan(scanner.ScanInts)
	if tok == scanner.EOF {
		sc.err = errors.New("unexpected EOF")
		return 0
	}

	if tok == '-' {
		negate = true
		tok = sc.scan(scanner.ScanInts)
	}

	num, err := strconv.Atoi(sc.scanner.TokenText())
	sc.err = err

	if negate {
		num = -num
	}

	return num
}

func (sc *Scanner) ConsumeString(s string) {
	if sc.err != nil {
		return
	}

	if sc.ScanString() != s {
		sc.err = errors.New("unexpected string")
	}
}

func (sc *Scanner) ConsumeRune(ch rune) {
	if sc.err != nil {
		return
	}

	if sc.ScanRune() != ch {
		sc.err = errors.New("unexpected rune")
	}
}

func (sc *Scanner) ConsumeInt(num int) {
	if sc.err != nil {
		return
	}

	if sc.ScanInt() != num {
		sc.err = errors.New("unexpected integer")
	}
}

func (sc *Scanner) Scan(v interface{}) {
	if p, ok := v.(*string); ok {
		*p = sc.ScanString()
		return
	}

	if p, ok := v.(*rune); ok {
		*p = sc.ScanRune()
		return
	}

	if p, ok := v.(*int); ok {
		*p = sc.ScanInt()
		return
	}

	if want, ok := v.(string); ok {
		sc.ConsumeString(want)
		return
	}

	if want, ok := v.(rune); ok {
		sc.ConsumeRune(want)
		return
	}

	if want, ok := v.(int); ok {
		sc.ConsumeInt(want)
		return
	}

	panic("unrecognized argument")
}

func Scan(s string, v ...interface{}) error {
	sc := New(s)

	for i := range v {
		sc.Scan(v[i])
	}

	return sc.Stop()
}
