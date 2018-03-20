package main

import (
	"errors"
	"fmt"
	"os"
	"reflect"
	"strconv"
)

var (
	ErrPointer = errors.New("expected a valid pointer")
	ErrStruct  = errors.New("expected a struct")
)

// LoadStruct parses strings in a map and stores values to struct fields that
// have the same tags as the map keys. Returns an error if dst is not a pointer
// to a struct or if any parse error is occured.
func LoadStruct(src map[string]string, dst interface{}, tagKey string) error {
	pval := reflect.ValueOf(dst)
	if !pval.IsValid() || pval.Type().Kind() != reflect.Ptr {
		return ErrPointer
	}

	val := reflect.Indirect(pval)
	typ := val.Type()
	if typ.Kind() != reflect.Struct {
		return ErrStruct
	}

	for i := 0; i < typ.NumField(); i++ {
		tag := typ.Field(i).Tag.Get(tagKey)

		str, ok := src[tag]
		if !ok {
			continue
		}

		if err := set(val.Field(i), str); err != nil {
			return err
		}
	}
	return nil
}

func set(val reflect.Value, str string) error {
	switch val.Interface().(type) {
	case string:
		val.SetString(str)

	case int:
		num, err := strconv.ParseInt(str, 10, 64)
		if err != nil {
			return err
		}
		val.SetInt(num)
	}
	return nil
}

func main() {
	type Resource struct {
		User      string `foo:"user"`
		Queue     string `foo:"queue"`
		HostCount int    `foo:"host_count"`
	}

	src := map[string]string{
		"user":       "alice",
		"host_count": "10",
	}
	res := Resource{}

	if err := LoadStruct(src, &res, "foo"); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
	}

	fmt.Println(res)
}
