package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Job struct {
	Walltime int
	CPUTime  int
}

func main() {
	var job Job

	job.Walltime, _ = parseClock("12:34:56")
	job.CPUTime, _ = parseClock("42")

	fmt.Println(job.Walltime)
	fmt.Println(job.CPUTime)
}

// parseClock parses a time string of the form [[hh:]mm:]ss and returns the time
// represented by the string in seconds.
func parseClock(s string) (int, error) {
	clock := 0

	for s != "" {
		component, rest := splitOnce(s, ":")

		n, err := strconv.Atoi(component)
		if err != nil {
			return 0, err
		}

		clock *= 60
		clock += int(n)

		s = rest
	}

	return clock, nil
}

// splitOnce splits s at the first occurrence of sep.
func splitOnce(s, sep string) (string, string) {
	n := strings.Index(s, sep)
	if n == -1 {
		return s, ""
	}
	return s[:n], s[n+len(sep):]
}
