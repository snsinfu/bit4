package qstat

import (
	"errors"
	"strings"
)

type (
	RawTable  map[string]RawRecord
	RawRecord map[string]string
)

const (
	tokRecordStart    = "Job Id:"
	tokFieldDelimiter = "="
)

var (
	ErrBadInput = errors.New("bad input")
)

// ParseRawTable parses `qstat -f1` output and returns a rawTable.
func ParseRawTable(src string) (RawTable, error) {
	type parserState int

	const (
		stateInit parserState = iota
		stateRow
	)

	table := RawTable{}
	state := stateInit
	jobID := ""

	for _, line := range strings.Split(src, "\n") {
		switch state {
		case stateInit:
			if strings.HasPrefix(line, tokRecordStart) {
				state = stateRow
				jobID = strings.TrimSpace(strings.TrimPrefix(line, tokRecordStart))
				table[jobID] = RawRecord{}
			}

		case stateRow:
			if line == "" {
				state = stateInit
				break
			}

			fields := strings.SplitN(line, tokFieldDelimiter, 2)
			if len(fields) != 2 {
				return table, ErrBadInput
			}

			key := strings.TrimSpace(fields[0])
			value := strings.TrimSpace(fields[1])
			if key == "" {
				return table, ErrBadInput
			}

			table[jobID][key] = value
		}
	}
	return table, nil
}

// ParseJobs parses qstat output and creates a map of Jobs.
func ParseJobs(qstat string) (map[string]Job, error) {
	table, err := ParseRawTable(qstat)
	if err != nil {
		return nil, err
	}

	jobs := map[string]Job{}
	for id, rec := range table {
		job, err := LoadJob(rec)
		if err != nil {
			return jobs, err
		}
		jobs[id] = job
	}
	return jobs, nil
}
