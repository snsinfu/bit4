package main

import (
	"testing"
	"time"
)

func TestParseUnixTime(t *testing.T) {
	testCases := []struct{
		Str  string
		Time time.Time
	}{
		{"1516039493", time.Unix(1516039493, 0)},
		{"1516039493.", time.Unix(1516039493, 0)},
		{"1516039493.00", time.Unix(1516039493, 0)},
		{"1516039493.001033", time.Unix(1516039493, 1033000)},
		{"1516039493.001033554", time.Unix(1516039493, 1033554)},
		{"1516039493.001033554000", time.Unix(1516039493, 1033554)},
	}

	for _, testCase := range testCases {
		tm, err := ParseUnixTime(testCase.Str)

		if err != nil {
			t.Errorf("unexpected error: %v", err)
		}

		if tm != testCase.Time {
			t.Errorf("unexpected result: %v", tm)
		}
	}
}

func TestParseUnixTime_errorCheck(t *testing.T) {
	_, err := ParseUnixTime("1516039493.001033554***")
	if err == nil {
		t.Error("error is expected")
	}
}
