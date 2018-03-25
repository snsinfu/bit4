package strmap

import (
	"reflect"
)

type Encodable interface {
	Encode(v reflect.Value) string
}

type Decodable interface {
	Decode(s string, v reflect.Value) error
}

func structOf(v interface{}) (reflect.Value, reflect.Type) {
	pval := reflect.ValueOf(v)
	if !pval.IsValid() || pval.Type().Kind() != reflect.Ptr {
		panic("expected a pointer")
	}

	val := reflect.Indirect(pval)
	typ := val.Type()
	if typ.Kind() != reflect.Struct {
		panic("expected a struct")
	}
	return val, typ
}

func Encode(v Encodable, tag string) map[string]string {
	val, typ := structOf(v)
	result := map[string]string{}

	for i := 0; i < typ.NumField(); i++ {
		key := typ.Field(i).Tag.Get(tag)
		result[key] = v.Encode(val.Field(i))
	}
	return result
}

func Decode(s map[string]string, v Decodable, tag string) error {
	val, typ := structOf(v)

	for i := 0; i < typ.NumField(); i++ {
		key := typ.Field(i).Tag.Get(tag)
		str, ok := s[key]
		if !ok {
			continue
		}
		if err := v.Decode(str, val.Field(i)); err != nil {
			return err
		}
	}
	return nil
}
