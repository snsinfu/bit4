package main

import (
	"testing"
)

const benchCase = "1516033182.796257011"

func BenchmarkParseTimestampFloat64(b *testing.B) {
	for i := 0; i < b.N; i++ {
		ParseTimestampFloat64(benchCase)
	}
}

func BenchmarkParseTimestampDecimal(b *testing.B) {
	for i := 0; i < b.N; i++ {
		ParseTimestampDecimal(benchCase)
	}
}

func BenchmarkParseTimestamp2xInt64(b *testing.B) {
	for i := 0; i < b.N; i++ {
		ParseTimestamp2xInt64(benchCase)
	}
}

func BenchmarkParseTimestampIntFloat(b *testing.B) {
	for i := 0; i < b.N; i++ {
		ParseTimestampIntFloat(benchCase)
	}
}
