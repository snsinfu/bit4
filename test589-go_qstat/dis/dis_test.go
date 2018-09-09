package dis

import (
	"bufio"
	"strings"
	"testing"
)

func TestEncodeInt(t *testing.T) {
	testCases := []struct {
		input    int64
		expected string
	}{
		{0, "+0"},
		{1, "+1"},
		{9, "+9"},
		{10, "2+10"},
		{100, "3+100"},
		{1234567890, "210+1234567890"},
		{-1, "-1"},
		{-9, "-9"},
		{-10, "2-10"},
		{-100, "3-100"},
		{-1234567890, "210-1234567890"},
	}

	for _, testCase := range testCases {
		actual := EncodeInt(testCase.input)
		if actual != testCase.expected {
			t.Errorf("unexpected result: got %q, want %q", actual, testCase.expected)
		}
	}
}

func TestEncodeString(t *testing.T) {
	testCases := []struct {
		input    string
		expected string
	}{
		{"", "+0"},
		{"A", "+1A"},
		{"Lorem", "+5Lorem"},
		{"LoremIpsum", "2+10LoremIpsum"},
	}

	for _, testCase := range testCases {
		actual := EncodeString(testCase.input)
		if actual != testCase.expected {
			t.Errorf("unexpected result: got %q, want %q", actual, testCase.expected)
		}
	}
}

func TestReadInt(t *testing.T) {
	testCases := []struct {
		input    string
		expected int64
		remain   string
	}{
		{"+05", 0, "5"},
		{"+15", 1, "5"},
		{"+95", 9, "5"},
		{"2+105", 10, "5"},
		{"3+1005", 100, "5"},
		{"210+12345678905", 1234567890, "5"},
		{"-15", -1, "5"},
		{"-95", -9, "5"},
		{"2-105", -10, "5"},
		{"3-1005", -100, "5"},
		{"210-12345678905", -1234567890, "5"},
	}

	for _, testCase := range testCases {
		r := bufio.NewReader(strings.NewReader(testCase.input))
		actual, err := ReadInt(r)

		if err != nil {
			t.Errorf("unexpected error for %q: %v", testCase.input, err)
			continue
		}

		if actual != testCase.expected {
			t.Errorf("unexpected result: got %d, want %d", actual, testCase.expected)
		}

		remain := make([]byte, len(testCase.input))
		n, err := r.Read(remain)

		if err != nil {
			t.Errorf("unexpected error: %v", err)
			continue
		}

		if string(remain[:n]) != testCase.remain {
			t.Errorf("unexpected trailing string: got %q, want %q", remain, testCase.remain)
		}
	}
}

func TestReadInt_rejectsTooLargeNumber(t *testing.T) {
	badInputs := []string{
		"219+9223372036854775808", // int64 max + 1
		"220+12345678901234567890",
		"22012345678901234567890+0",
	}

	for _, input := range badInputs {
		r := bufio.NewReader(strings.NewReader(input))
		actual, err := ReadInt(r)

		if err != nil {
			continue // OK
		}

		t.Errorf("unexpected success: input %q output %d", input, actual)
	}
}

func TestReadInt_rejectsInvalidInput(t *testing.T) {
	badInputs := []string{
		// Bad prefix
		"a+1",
		"+++",
		" 1",
		"310+1234567890",

		// Bad number
		"+a",
		"3+1a0",

		// EOF
		"",
		"2",
		"2+",
		"2-",
		"2+1",
		"3+10",
	}

	for _, input := range badInputs {
		r := bufio.NewReader(strings.NewReader(input))
		actual, err := ReadInt(r)

		if err != nil {
			continue // OK
		}

		t.Errorf("unexpected success: input %q output %d", input, actual)
	}
}

func TestReadString(t *testing.T) {
	testCases := []struct {
		input    string
		expected string
		remain   string
	}{
		{"+0Z", "", "Z"},
		{"+1AZ", "A", "Z"},
		{"+5LoremZ", "Lorem", "Z"},
		{"2+10LoremIpsumZ", "LoremIpsum", "Z"},
	}

	for _, testCase := range testCases {
		r := bufio.NewReader(strings.NewReader(testCase.input))
		actual, err := ReadString(r)

		if err != nil {
			t.Errorf("unexpected error for %q: %v", testCase.input, err)
			continue
		}

		if actual != testCase.expected {
			t.Errorf("unexpected result: got %q, want %q", actual, testCase.expected)
		}

		remain := make([]byte, len(testCase.input))
		n, err := r.Read(remain)

		if err != nil {
			t.Errorf("unexpected error: %v", err)
			continue
		}

		if string(remain[:n]) != testCase.remain {
			t.Errorf("unexpected trailing string: got %q, want %q", remain, testCase.remain)
		}
	}
}

func TestReadString_rejectsInvalidInput(t *testing.T) {
	badInputs := []string{
		// Bad prefix
		"a+1",
		"+++",
		" 1",
		"310+1234567890",

		// EOF
		"",
		"+1",
		"2+10abcdefghj",
	}

	for _, input := range badInputs {
		r := bufio.NewReader(strings.NewReader(input))
		actual, err := ReadString(r)

		if err != nil {
			continue // OK
		}

		t.Errorf("unexpected success: input %q output %q", input, actual)
	}
}
