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
	comment := "#"
	delimiter := "\t"

	scanner := bufio.NewScanner(os.Stdin)

	// Header
	var columns []string

	for scanner.Scan() {
		line := scanner.Text()

		if strings.HasPrefix(line, comment) {
			continue
		}

		columns = strings.Split(line, delimiter)
		break
	}

	fmt.Println(strings.Join(columns, delimiter))

	// Data
	var curGroup string

	for scanner.Scan() {
		record := strings.Split(scanner.Text(), delimiter)

		group := record[0]
		_, err1 := strconv.ParseInt(record[1], 10, 64)
		_, err2 := strconv.ParseInt(record[2], 10, 64)
		_, err3 := strconv.ParseFloat(record[3], 64)

		if err := firstErr(err1, err2, err3); err != nil {
			return err
		}

		if group != curGroup {
			fmt.Println(group)
			curGroup = group
		}
	}

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
