package strmap

import (
	"reflect"
	"testing"
)

func assertMapEq(t *testing.T, actual, expected map[string]interface{}) {
	t.Helper()

	if len(actual) != len(expected) {
		t.Errorf(
			"unexpected length\nExpect: %d (%v)\nActual: %d (%v)",
			len(expected), expected, len(actual), actual,
		)
	}

	for key, val := range expected {
		if !reflect.DeepEqual(actual[key], val) {
			t.Errorf(
				"value for '%s' mismatch\nExpect: %v\nActual: %v",
				key, val, actual[key],
			)
		}
	}
}

func assertEq(t *testing.T, actual, expected interface{}) {
	if !reflect.DeepEqual(actual, expected) {
		t.Errorf("unexpected value\nExpect: %v\nActual: %v", expected, actual)
	}
}

func TestMap_works_with_struct(t *testing.T) {
	type Foo struct {
		Id    int               `bar:"id"`
		Name  string            `bar:"name"`
		Props map[string]string `bar:"props"`
		Cache string            `qux:"cache"`
	}

	foo := Foo{
		Id:    12,
		Name:  "foo",
		Props: map[string]string{"a": "apple", "b": "banana"},
		Cache: "this field should not be mapped",
	}

	fooMap := map[string]interface{}{
		"id":    foo.Id,
		"name":  foo.Name,
		"props": foo.Props,
	}

	assertMapEq(t, Map(&foo, "bar"), fooMap)
}

func TestMap_works_with_nested_struct(t *testing.T) {
	type Foo struct {
		Id   int    `json:"id"`
		Name string `json:"name"`
	}

	type Bar struct {
		Id  int `json:"id"`
		Foo Foo `json:"foo"`
	}

	bar := Bar{
		Id: 42,
		Foo: Foo{
			Id:   24,
			Name: "alice",
		},
	}

	barMap := map[string]interface{}{
		"id": bar.Id,
		"foo": map[string]interface{}{
			"id":   bar.Foo.Id,
			"name": bar.Foo.Name,
		},
	}

	assertMapEq(t, Map(&bar, "json"), barMap)
}

func TestMap_works_with_embedded_type(t *testing.T) {
	type Foo struct {
		Id int `json:"id"`
	}

	type Bar struct {
		Foo  `json:"foo"`
		Name string `json:"name"`
	}

	bar := Bar{
		Foo: Foo{
			Id: 42,
		},
		Name: "alice",
	}

	barMap := map[string]interface{}{
		"name": bar.Name,
		"foo": map[string]interface{}{
			"id": bar.Foo.Id,
		},
	}

	assertMapEq(t, Map(&bar, "json"), barMap)
}

func TestUnmap_works_with_struct(t *testing.T) {
	type Foo struct {
		Id    int               `bar:"id"`
		Name  string            `bar:"name"`
		Props map[string]string `bar:"props"`
		Cache string            `qux:"cache"`
	}

	foo := Foo{
		Id:    12,
		Name:  "foo",
		Props: map[string]string{"a": "apple", "b": "banana"},
	}

	fooMap := map[string]interface{}{
		"id":    foo.Id,
		"name":  foo.Name,
		"props": foo.Props,
		"cache": "this field should not be unmapped",
	}

	var out Foo
	Unmap(fooMap, &out, "bar")

	assertEq(t, out, foo)
}

func TestUnmap_works_with_nested_struct(t *testing.T) {
	type Foo struct {
		Id   int    `json:"id"`
		Name string `json:"name"`
	}

	type Bar struct {
		Id  int `json:"id"`
		Foo Foo `json:"foo"`
	}

	bar := Bar{
		Id: 42,
		Foo: Foo{
			Id:   24,
			Name: "alice",
		},
	}

	barMap := map[string]interface{}{
		"id": bar.Id,
		"foo": map[string]interface{}{
			"id":   bar.Foo.Id,
			"name": bar.Foo.Name,
		},
	}

	var out Bar
	Unmap(barMap, &out, "json")

	assertEq(t, out, bar)
}

func TestUnmap_works_with_embedded_type(t *testing.T) {
	type Foo struct {
		Id int `json:"id"`
	}

	type Bar struct {
		Foo  `json:"foo"`
		Name string `json:"name"`
	}

	bar := Bar{
		Foo: Foo{
			Id: 42,
		},
		Name: "alice",
	}

	barMap := map[string]interface{}{
		"name": bar.Name,
		"foo": map[string]interface{}{
			"id": bar.Foo.Id,
		},
	}

	var out Bar
	Unmap(barMap, &out, "json")

	assertEq(t, out, bar)
}
