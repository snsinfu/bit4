package main

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"strconv"
	"strings"

	"github.com/shopspring/decimal"
)

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	in := strings.NewReader("52331433 1512315600 1297999.0 0.09980916\n")

	scaleY, err := decimal.NewFromString("100000000")
	if err != nil {
		return err
	}

	scanner := bufio.NewScanner(in)
	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) != 4 {
			return errors.New("Malformed input")
		}

		serialID, err1 := strconv.ParseInt(fields[0], 10, 64)
		timestamp, err2 := strconv.ParseFloat(fields[1], 64)
		valueX, err3 := decimal.NewFromString(fields[2])
		valueY, err4 := decimal.NewFromString(fields[3])
		if err := firstErr(err1, err2, err3, err4); err != nil {
			return err
		}

		valueY = valueY.Mul(scaleY)

		fmt.Printf("%d\t%.3f\t%s\t%s\n", serialID, timestamp, valueX, valueY)
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
