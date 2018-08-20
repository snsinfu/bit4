package main

import (
	"testing"
)

func BenchmarkMakeKeyPBKDF2_100000(b *testing.B) {
	for i := 0; i < b.N; i++ {
		makeKeyPBKDF2("Lorem ipsum dolor sit amet", 100000)
	}
}

func BenchmarkMakeKeyScrypt_32768(b *testing.B) {
	for i := 0; i < b.N; i++ {
		makeKeyScrypt("Lorem ipsum dolor sit amet", 32768)
	}
}
