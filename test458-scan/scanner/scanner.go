package scanner

import (
	"errors"
	"strconv"
	"strings"
	"text/scanner"
)

// Scanner holds source and error.
type Scanner struct {
	scanner scanner.Scanner
	err     error
}

// New creates a new Scanner to scan given string.
func New(s string) *Scanner {
	sc := &Scanner{}
	sc.scanner.Init(strings.NewReader(s))
	sc.scanner.Error = func(_ *scanner.Scanner, msg string) {
		sc.err = errors.New(msg)
	}
	sc.scanner.Mode = scanner.ScanIdents | scanner.ScanInts
	return sc
}

// Err returns the first error encountered by the Scanner.
func (sc *Scanner) Err() error {
	return sc.err
}

// ScanIdent scans a next token in the source string as a Go identifier. Scan
// is not performed if Err is not nil.
func (sc *Scanner) ScanIdent() string {
	if sc.err != nil {
		return ""
	}

	if sc.scanner.Scan() != scanner.Ident {
		sc.err = errors.New("identifier is expected")
	}

	return sc.scanner.TokenText()
}

// ScanInt scans a next token in the source string as a non-negative decimal
// integer. Scan is not performed if Err is not nil.
func (sc *Scanner) ScanInt() int {
	if sc.err != nil {
		return 0
	}

	if sc.scanner.Scan() != scanner.Int {
		sc.err = errors.New("integer is expected")
	}

	num, err := strconv.Atoi(sc.scanner.TokenText())
	sc.err = err

	return num
}

// Consume scans a next token in the source string as a non-identifier rune c.
// Scan is not performed if Err is not nil.
func (sc *Scanner) Consume(c rune) rune {
	if sc.err != nil {
		return 0
	}

	tok := sc.scanner.Scan()
	if tok != c {
		sc.err = errors.New("unexpected input")
	}

	return tok
}

// Stop expects the end of the source string. Returns the first error, if any,
// encountered by the Scanner.
func (sc *Scanner) Stop() error {
	if sc.err != nil {
		return sc.err
	}

	if sc.scanner.Scan() != scanner.EOF {
		sc.err = errors.New("EOF is expected")
	}

	return sc.err
}
