package strmap

import (
	"reflect"
	"strconv"
	"testing"
)

type testPerson struct {
	Id   int    `foo:"id"`
	Name string `foo:"name"`
}

func (p *testPerson) Encode(v reflect.Value) string {
	switch v.Interface().(type) {
	case int:
		return strconv.Itoa(int(v.Int()))

	case string:
		return v.String()
	}
	return ""
}

func (p *testPerson) Decode(s string, v reflect.Value) error {
	switch v.Interface().(type) {
	case int:
		i, err := strconv.Atoi(s)
		if err != nil {
			return err
		}
		v.SetInt(int64(i))

	case string:
		v.SetString(s)
	}
	return nil
}

func TestEncode(t *testing.T) {
	person := testPerson{
		Id:   1234,
		Name: "Quick-Brown Fox",
	}
	expected := map[string]string{
		"id":   "1234",
		"name": "Quick-Brown Fox",
	}
	actual := Encode(&person, "foo")

	if !reflect.DeepEqual(actual, expected) {
		t.Errorf("Unexpected result\nActual: %v\nExpected: %v", actual, expected)
	}
}

func TestDecode(t *testing.T) {
	code := map[string]string{
		"id":   "1234",
		"name": "Quick-Brown Fox",
	}
	expected := testPerson{
		Id:   1234,
		Name: "Quick-Brown Fox",
	}
	actual := testPerson{}
	err := Decode(code, &actual, "foo")

	if err != nil {
		t.Errorf("Unexpected error: '%v'", err)
	}

	if !reflect.DeepEqual(actual, expected) {
		t.Errorf("Unexpected result\nActual: %v\nExpected: %v", actual, expected)
	}
}
