package qstat

import (
	"testing"
)

func Test_loadAttr_int(t *testing.T) {
	var actual int
	if err := loadAttr(valueRef(&actual), "123"); err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}
	expected := 123

	if actual != expected {
		t.Errorf(
			"Value is not properly set: %d, expected %d",
			actual,
			expected,
		)
	}
}

func Test_loadAttr_string(t *testing.T) {
	var actual string
	if err := loadAttr(valueRef(&actual), "foobar"); err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}
	expected := "foobar"

	if actual != expected {
		t.Errorf(
			"Value is not properly set: '%s', expected '%s'",
			actual,
			expected,
		)
	}
}

func Test_LoadJob(t *testing.T) {
	record := RawRecord{
		"Job_Name":   "myjob:1",
		"Job_Owner":  "alice",
		"job_state":  "R",
		"session_id": "123",
	}

	job, err := LoadJob(record)
	if err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}

	if job.Name != "myjob:1" {
		t.Errorf("Name is not properly set: '%s', expected '%s'", job.Name, "myjob:1")
	}

	if job.Owner != "alice" {
		t.Errorf("Owner is not properly set: '%s', expected '%s'", job.Owner, "alice")
	}

	if job.State != "R" {
		t.Errorf("State is not properly set: '%s', expected '%s'", job.State, "R")
	}

	if job.SessionID != 123 {
		t.Errorf("SessionID is not properly set: %d, expected %d", job.SessionID, 123)
	}
}
