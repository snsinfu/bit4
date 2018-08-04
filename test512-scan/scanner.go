package main

import (
	"errors"
	"strconv"
	"strings"
	"unicode"
)

type scanner struct {
	src string
	err error
}

var (
	errNoExpectedString = errors.New("no expected string")
	errNoExpectedInt    = errors.New("no expected integer")
	errUnparsedEnd      = errors.New("unparsed string at the end")
)

func newScanner(src string) scanner {
	return scanner{src: src}
}

func (s *scanner) Scan(v interface{}) {
	if s.err != nil {
		return
	}
	s.prepare()

	switch p := v.(type) {
	case *string:
		*p = s.scanString()

	case *int:
		*p = s.scanInt()

	case string:
		s.expectString(p)

	case int:
		if s.scanInt() != p {
			s.err = errNoExpectedInt
		}
	}
}

func (s *scanner) prepare() {
	s.src = strings.TrimLeftFunc(s.src, unicode.IsSpace)
}

func (s *scanner) scanString() string {
	end := strings.IndexAny(s.src, ":/{}[]")
	if end == -1 {
		end = len(s.src)
	}
	str := s.src[:end]
	s.src = s.src[end:]
	return str
}

func (s *scanner) scanInt() int {
	end := 0
	if len(s.src) != 0 && (s.src[0] == '+' || s.src[0] == '-') {
		end++
	}
	for end < len(s.src) {
		if s.src[end] < '0' || '9' < s.src[end] {
			break
		}
		end++
	}

	num, err := strconv.Atoi(s.src[:end])
	if err != nil {
		s.err = err
		return 0
	}

	s.src = s.src[end:]
	return num
}

func (s *scanner) expectString(p string) {
	if len(p) == 0 {
		return
	}

	trimmed := strings.TrimPrefix(s.src, p)
	if len(trimmed) == len(s.src) {
		s.err = errNoExpectedString
		return
	}

	s.src = trimmed
}

func (s *scanner) Finish() error {
	if s.err != nil {
		return s.err
	}
	s.prepare()

	if len(s.src) != 0 {
		s.err = errUnparsedEnd
		return s.err
	}

	return nil
}
