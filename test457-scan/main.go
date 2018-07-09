package main

import (
	"fmt"
	"strings"
	"text/scanner"
)

func main() {
	s := scanner.Scanner{}
	s.Init(strings.NewReader("443/tcp"))
	s.Error = func(_ *scanner.Scanner, _ string) {}
	s.Mode = scanner.ScanIdents | scanner.ScanInts

	for tok := s.Scan(); tok != scanner.EOF; tok = s.Scan() {
		fmt.Println(tok, s.TokenText())
	}
}
