package scanner

import (
	"fmt"
)

func Example_PortProto() {
	sc := New("80/tcp")

	port := sc.ScanInt()
	sc.Consume('/')
	proto := sc.ScanIdent()

	if err := sc.Stop(); err != nil {
		fmt.Println("error:", err)
	}

	fmt.Println(port)
	fmt.Println(proto)

	// Output:
	// 80
	// tcp
}
