package main

import (
	"strconv"
	"strings"
	"time"
)

const decimalSeparator = "."
const decimalPrecision = 9

func ParseUnixTime(s string) (time.Time, error) {
	intpart, fracpart := splitDecimalParts(s)

	sec, err := parseWholeSecond(intpart)
	if err != nil {
		return time.Time{}, err
	}

	nsec, err := parseFractionalSecond(fracpart)
	if err != nil {
		return time.Time{}, err
	}

	return time.Unix(sec, nsec), nil
}

func splitDecimalParts(s string) (string, string) {
	sep := strings.Index(s, decimalSeparator)
	if sep == -1 {
		return s, ""
	}
	return s[:sep], s[sep + 1:]
}

func parseWholeSecond(s string) (int64, error) {
	if len(s) == 0 {
		return 0, nil
	}
	return strconv.ParseInt(s, 10, 64)
}

func parseFractionalSecond(s string) (int64, error) {
	if len(s) == 0 {
		return 0, nil
	}
	if len(s) > decimalPrecision {
		s = s[:decimalPrecision]
	}

	num, err := strconv.ParseInt(s, 10, 64)
	if err != nil {
		return 0, nil
	}

	for i := len(s); i < decimalPrecision; i++ {
		num *= 10
	}
	return num, nil
}
