package main

import (
	"fmt"
)

func main() {
	v := ValueFoo{}
	doFoo(v)
	if v.changed { panic("impossible") }
	doFoo(&v)
	if v.changed { panic("impossible") }

	// Cannot call doFoo(p)
	p := PtrFoo{}
	doFoo(&p)
	if !p.changed { panic("impossible") }
}

func doFoo(f interface{Foo()}) {
	f.Foo()
}

type ValueFoo struct {
	changed bool
}

func (v ValueFoo) Foo() {
	fmt.Println("Hello via value")
	v.changed = true
}

type PtrFoo struct {
	changed bool
}

func (p *PtrFoo) Foo() {
	fmt.Println("Hello via ptr")
	p.changed = true
}
