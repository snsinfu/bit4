package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"

	"github.com/snsinfu/bit4/test251-rebin_interval/rebin"
)

var (
	errUnexpectedFieldCount = errors.New("unexpected field count")
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
	}
}

func run() error {
	err := scanPulses(os.Stdin, func(series string, pulse rebin.Bin) error {
		return nil
	})
	return err
}

func scanPulses(in io.Reader, yield func(string, rebin.Bin) error) error {
	buf := bufio.NewScanner(os.Stdin)

	for buf.Scan() {
		record := strings.Fields(buf.Text())
		if len(record) != 4 {
			return errUnexpectedFieldCount
		}

		series := record[0]

		beg, err := strconv.ParseInt(record[1], 10, 64)
		if err != nil {
			return err
		}

		end, err := strconv.ParseInt(record[2], 10, 64)
		if err != nil {
			return err
		}

		value, err := strconv.ParseFloat(record[3], 64)
		if err != nil {
			return err
		}

		if err := yield(series, rebin.Bin{beg, end, value}); err != nil {
			return err
		}
	}

	return nil
}
