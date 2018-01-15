package main

import (
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/shopspring/decimal"
)

var decimalOne = decimal.New(1, 0)

func main() {
	s := "1516033182.796257011"
	fmt.Println(ParseTimestampFloat64(s))
	fmt.Println(ParseTimestampDecimal(s))
	fmt.Println(ParseTimestamp2xInt64(s))
	fmt.Println(ParseTimestampIntFloat(s))
}

func ParseTimestampFloat64(s string) time.Time {
	sec, err := strconv.ParseFloat(s, 64)
	check(err)
	_, subsec := math.Modf(sec)
	nsec := subsec * 1e9
	return time.Unix(int64(sec), int64(nsec))
}

func ParseTimestampDecimal(s string) time.Time {
	sec, err := decimal.NewFromString(s)
	check(err)
	wholesec, subsec := sec.QuoRem(decimalOne, 0)
	nsec := subsec.Mul(decimal.New(1, 9))
	return time.Unix(wholesec.IntPart(), nsec.IntPart())
}

func ParseTimestamp2xInt64(s string) time.Time {
	secStr, subsecStr := decimalParts(s)

	if len(subsecStr) > 9 {
		subsecStr = subsecStr[:9]
	}
	scale := 9 - len(subsecStr)

	sec, err := strconv.ParseInt(secStr, 10, 64)
	check(err)

	subsec, err := strconv.ParseInt(subsecStr, 10, 64)
	check(err)

	for i := 0; i < scale; i++ {
		subsec *= 10
	}

	return time.Unix(sec, subsec)
}

func decimalParts(s string) (string, string) {
	dp := strings.Index(s, ".")
	if dp == -1 {
		return s, ""
	}
	return s[:dp], s[(dp + 1):]
}

func ParseTimestampIntFloat(s string) time.Time {
	dp := strings.Index(s, ".")
	if dp == -1 {
		dp = len(s)
	}

	sec, err := strconv.ParseInt(s[:dp], 10, 64)
	check(err)

	subsec, err := strconv.ParseFloat(s[dp:], 64)
	check(err)

	return time.Unix(sec, int64(subsec * 1e9))
}

func check(err error) {
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
