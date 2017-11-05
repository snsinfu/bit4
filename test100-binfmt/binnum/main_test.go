package main

import (
	"testing"
)

func TestReformatAsBinary(t *testing.T) {
	testCases := []struct {
		input, expected string
	}{
		{"0", "0"},
		{"1", "1"},
		{"10", "1010"},
		{"100", "1100100"},
		{"256", "100000000"},
		{"15187", "11101101010011"},
		{"25209", "110001001111001"},
	}
	for _, testCase := range testCases {
		actual, err := ReformatAsBinary(testCase.input)
		if err != nil {
			t.Errorf("%v => unexpected error %v", testCase.input, err)
		}
		if actual != testCase.expected {
			t.Errorf("%v => %v, want %v", testCase.input, actual, testCase.expected)
		}
	}
}

func TestReformatAsBinary_Errors(t *testing.T) {
	testCases := []string{
		"", "x", "[", "123x", "123[", " 123", "123 ",
	}
	for _, testCase := range testCases {
		if _, err := ReformatAsBinary(testCase); err == nil {
			t.Errorf("%v => error is expected", testCase)
		}
	}
}
