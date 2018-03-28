package strmap

import (
	"reflect"
)

type StructPtr interface{}

func structValue(sp StructPtr) reflect.Value {
	return reflect.Indirect(reflect.ValueOf(sp))
}

func Map(sp StructPtr, tag string) map[string]interface{} {
	s := structValue(sp)
	t := s.Type()
	m := map[string]interface{}{}

	for i := 0; i < s.NumField(); i++ {
		key := t.Field(i).Tag.Get(tag)
		if key == "" {
			continue
		}
		field := s.Field(i)

		if field.Type().Kind() == reflect.Struct {
			m[key] = Map(field.Addr().Interface(), tag)
		} else {
			m[key] = field.Interface()
		}
	}
	return m
}

func Unmap(m map[string]interface{}, sp StructPtr, tag string) {
	s := structValue(sp)
	t := s.Type()

	for i := 0; i < s.NumField(); i++ {
		key := t.Field(i).Tag.Get(tag)
		if key == "" {
			continue
		}
		field := s.Field(i)

		val, ok := m[key]
		if !ok {
			continue
		}

		if field.Type().Kind() == reflect.Struct {
			submap := val.(map[string]interface{})
			Unmap(submap, field.Addr().Interface(), tag)
		} else {
			field.Set(reflect.ValueOf(val))
		}
	}
}
