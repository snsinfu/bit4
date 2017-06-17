package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func run() error {

	binSize := int64(8)

	scanner := bufio.NewScanner(os.Stdin)
	delimiter := "\t"

	curEnd := binSize
	curVal := float64(0)

	output := func(start, end int64, value float64) {
		fmt.Printf("%d\t%d\t%g\n", start, end, value / float64(end - start))
	}

	for scanner.Scan() {
		record := strings.SplitN(scanner.Text(), delimiter, 3)

		start, err1 := strconv.ParseInt(record[0], 10, 64)
		end, err2 := strconv.ParseInt(record[1], 10, 64)
		val, err3 := strconv.ParseFloat(record[2], 64)

		if err := firstErr(err1, err2, err3); err != nil {
			return err
		}

		// Fill the gap
		for start >= curEnd {
			output(curEnd - binSize, curEnd, curVal)
			curEnd += binSize
			curVal = 0
		}

		// Consume input
		for end > curEnd {
			curVal += float64(curEnd - start) * val
			output(curEnd - binSize, curEnd, curVal)
			start = curEnd
			curEnd += binSize
			curVal = 0
		}

		// Accumulate remaining part
		curVal += float64(end - start) * val
	}
	output(curEnd - binSize, curEnd, curVal)

	return scanner.Err()
}

func firstErr(errs ...error) error {
	for _, err := range errs {
		if err != nil {
			return err
		}
	}
	return nil
}
