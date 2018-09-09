package dis

import (
	"bufio"
	"errors"
	"io"
	"strconv"
)

const (
	maxDigitCount = 19 // 64-bit signed integer
)

var (
	errIntOverflow = errors.New("integer overflow")
)

// EncodeInt returns a string encoding integer i in the DIS format.
func EncodeInt(i int64) string {
	repr := strconv.FormatInt(i, 10)
	if i >= 0 {
		repr = "+" + repr
	}
	count := int64(len(repr) - 1)

	for count > 1 {
		s := strconv.FormatInt(count, 10)
		count = int64(len(s))
		repr = s + repr
	}

	return repr
}

// EncodeString returns a string encoding string s in the DIS format.
func EncodeString(s string) string {
	return EncodeInt(int64(len(s))) + s
}

// ReadInt parses a DIS-encoded integer out of r.
func ReadInt(r *bufio.Reader) (int64, error) {
	count := 1
	buf := make([]byte, maxDigitCount+1)

	for {
		next, err := r.Peek(1)
		if err != nil {
			return 0, err
		}

		if next[0] == byte('+') || next[0] == byte('-') {
			count++
			break
		}

		if _, err := io.ReadFull(r, buf[:count]); err != nil {
			return 0, err
		}

		n, err := strconv.ParseInt(string(buf[:count]), 10, 64)
		if err != nil {
			return 0, err
		}

		count = int(n)

		if count > maxDigitCount {
			return 0, errIntOverflow
		}
	}

	if _, err := io.ReadFull(r, buf[:count]); err != nil {
		return 0, err
	}

	return strconv.ParseInt(string(buf[:count]), 10, 64)
}

// ReadString parses a DIS-encoded string out of r.
func ReadString(r *bufio.Reader) (string, error) {
	n, err := ReadInt(r)
	if err != nil {
		return "", err
	}

	buf := make([]byte, n)
	if _, err := io.ReadFull(r, buf); err != nil {
		return "", err
	}

	return string(buf), nil
}
