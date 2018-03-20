package qstat

import (
	"reflect"
	"testing"
)

func Test_ParseRawTable_canSplitJobs(t *testing.T) {
	qstat := `
Job Id: 1.example.com
    Job_Name = myjob:1

Job Id: 2.example.com
    Job_Name = myjob:2
`
	expected := RawTable{
		"1.example.com": {
			"Job_Name": "myjob:1",
		},
		"2.example.com": {
			"Job_Name": "myjob:2",
		},
	}
	actual, err := ParseRawTable(qstat)

	if err != nil {
		t.Errorf("Unexpected error: %v", err)
	}

	if len(actual) != len(expected) {
		t.Errorf(
			"Incorrect number of records: %d, expected %d",
			len(actual),
			len(expected),
		)
	}

	for jobID := range expected {
		_, ok := actual[jobID]
		if !ok {
			t.Errorf("No job record for '%s'", jobID)
		}

		if !reflect.DeepEqual(actual[jobID], expected[jobID]) {
			t.Errorf(
				"Incorrect job record: actual %v, expected %v",
				actual[jobID],
				expected[jobID],
			)
		}
	}
}

func Test_ParseRawTable_toleratesEmptyInput(t *testing.T) {
	table, err := ParseRawTable("")

	if err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}

	if len(table) != 0 {
		t.Errorf("Expected an empty result, got %v", table)
	}
}

func Test_ParseRawTable_toleratesEmptyLines(t *testing.T) {
	table, err := ParseRawTable("\n\n\n")

	if err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}

	if len(table) != 0 {
		t.Errorf("Expected an empty result, got %v", table)
	}
}

func Test_ParseJobs(t *testing.T) {
	qstat := `
Job Id: 1.example.com
    Job_Name = myjob:1
    job_state = R
    session_id = 123

Job Id: 2.example.com
    Job_Name = myjob:2
    job_state = Q
    session_id = 124
`
	expected := map[string]Job{
		"1.example.com": {
			Name:      "myjob:1",
			State:     "R",
			SessionID: 123,
		},
		"2.example.com": {
			Name:      "myjob:2",
			State:     "Q",
			SessionID: 124,
		},
	}
	actual, err := ParseJobs(qstat)

	if err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}

	for jobID := range expected {
		_, ok := actual[jobID]
		if !ok {
			t.Errorf("No job record for '%s'", jobID)
		}

		if !reflect.DeepEqual(actual[jobID], expected[jobID]) {
			t.Errorf(
				"Incorrect job record: actual %v, expected %v",
				actual[jobID],
				expected[jobID],
			)
		}
	}
}
