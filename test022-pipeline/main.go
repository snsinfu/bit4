package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"

	"github.com/frickiericker/bit4/test023-taskchan/taskch"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

type dataRow struct {
	start, end int64
	value      float64
}

func run() error {
	dataChan := make(chan dataRow)

	tch := taskch.New()

	tch.Go(func() error {
		return input(dataChan)
	})

	tch.Go(func() error {
		output(dataChan)
		return nil
	})

	return tch.Wait()
}

func output(inputChan <-chan dataRow) {
	sum := float64(0)
	cnt := float64(0)
	for record := range inputChan {
		width := float64(record.end - record.start)
		sum += width * record.value
		cnt += width
	}
	fmt.Println(sum / cnt)
}

func input(inputChan chan<- dataRow) error {
	defer close(inputChan)

	scanner := bufio.NewScanner(os.Stdin)
	delim := "\t"

	for scanner.Scan() {
		record := strings.SplitN(scanner.Text(), delim, 3)

		start, err1 := strconv.ParseInt(record[0], 10, 64)
		end, err2 := strconv.ParseInt(record[1], 10, 64)
		value, err3 := strconv.ParseFloat(record[2], 64)

		if err := firstErr(err1, err2, err3); err != nil {
			return err
		}

		inputChan <- dataRow{
			start: start,
			end:   end,
			value: value,
		}
	}
	return nil
}

func firstErr(errs ...error) error {
	for _, err := range errs {
		if err != nil {
			return err
		}
	}
	return nil
}
