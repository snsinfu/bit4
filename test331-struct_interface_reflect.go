package main

import (
	"fmt"
	"reflect"
)

func main() {
	type Point struct {
		X int
		Y int
	}
	p := Point{}

	set(&p)

	// set(p) causes a panic:
	//     panic: reflect: reflect.Value.SetInt using unaddressable value
	// even if the set() function is modified to use v := reflect.Valueof(s).

	fmt.Println(p)
}

func set(s interface{}) {
	v := reflect.Indirect(reflect.ValueOf(s))
	for i := 0; i < v.NumField(); i++ {
		val := v.Field(i)
		val.SetInt(1)
	}
}
