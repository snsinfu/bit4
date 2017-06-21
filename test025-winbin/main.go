package main

import (
	"fmt"
	"math"
)

func main() {

	input := []Bin{
		{Start: -10, End: -5, Dummy: true},
		{Start: -5, End: 0, Dummy: true},
		{Start: 0, End: 5, Value: 0.0},   // 0.0
		{Start: 5, End: 10, Value: 1.0},  // 5.0
		{Start: 10, End: 15, Value: 2.0}, // 10.0
		{Start: 15, End: 20, Value: 3.0}, // 15.0
		{Start: 20, End: 25, Value: 4.0}, // 20.0
		{Start: 25, End: 30, Value: 5.0}, // 25.0
		{Start: 30, End: 35, Value: 0.0}, // 0.0
		{Start: 35, End: 40, Value: 1.0}, // 5.0
		{Start: 40, End: 45, Value: 2.0}, // 10.0
		{Start: 45, End: 50, Value: 3.0}, // 15.0
		{Start: 50, End: 55, Value: 4.0}, // 20.0
		{Start: 55, End: 60, Value: 5.0}, // 25.0
		{Start: 60, End: 65, Dummy: true},
		{Start: 65, End: 70, Dummy: true},
	}

	// bin width = 15
	// win width = 35
	actual := roll(input, 3, 7)

	expected := []Bin{
		{Start: 0, End: 15, Value: 50. / 25},  // -10:25
		{Start: 15, End: 30, Value: 80. / 35}, // 5:40
		{Start: 30, End: 45, Value: 95. / 35}, // 20:55
		{Start: 45, End: 60, Value: 75. / 25}, // 35:70
	}

	if err := check(actual, expected); err != nil {
		fmt.Println(err)
	}
}

func check(actual []Bin, expected []Bin) error {

	if len(actual) != len(expected) {
		return fmt.Errorf(
			"lengths mismatch: got %d, expected %d",
			len(actual), len(expected))
	}

	epsilon := 1e-6

	for i := range expected {
		if actual[i].Start != expected[i].Start {
			return fmt.Errorf(
				"start mismatch at %d-th bin: got %d, expected %d",
				i, actual[i].Start, expected[i].Start)
		}

		if actual[i].End != expected[i].End {
			return fmt.Errorf(
				"end mismatch at %d-th bin: got %d, expected %d",
				i, actual[i].End, expected[i].End)
		}

		if math.Abs(actual[i].Value-expected[i].Value) > epsilon {
			return fmt.Errorf(
				"value mismatch at %d-th bin: got %g, expected %g",
				i, actual[i].Value, expected[i].Value)
		}
	}

	return nil
}

func roll(input []Bin, binSize, winSize int) []Bin {

	if gcd(binSize, winSize) != 1 {
		panic("precondition violated")
	}

	queue := new(BinQueue)
	idx := 0

	for i := 0; i < winSize - binSize; i++ {
		queue.Push(input[idx])
		idx++
	}

	output := []Bin{}

	for idx < len(input) {
		for i := 0; i < binSize; i++ {
			queue.Push(input[idx])
			idx++
		}

		window := queue.Contents()
		binStart := (winSize - binSize) / 2

		bin := Bin{
			Start: window[binStart].Start,
			End:   window[binStart+binSize-1].End,
			Value: mean(window),
		}
		output = append(output, bin)

		for i := 0; i < binSize; i++ {
			queue.Pop()
		}
	}

	return output
}

func mean(bins []Bin) float64 {
	sum := 0.0
	cnt := 0
	for _, bin := range bins {
		if bin.Dummy {
			continue
		}
		sum += bin.Value
		cnt++
	}
	return sum / float64(cnt)
}

func gcd(m, n int) int {
	for n > 0 {
		n, m = m%n, n
	}
	return m
}

// Bin represents an integer interval [Start .. End) annotated with a Value.
type Bin struct {
	Start, End int
	Value      float64
	Dummy      bool
}

// BinQueue is a queue of Bin values.
type BinQueue struct {
	storage    []Bin
	start, end int
}

// Size returns the number of values in the queue.
func (q *BinQueue) Size() int {
	return q.end - q.start
}

// Push inserts a value to the front of the queue.
func (q *BinQueue) Push(bin Bin) {
	if q.end == len(q.storage) {
		size := q.Size()

		storage := make([]Bin, 1+len(q.storage)*2)
		for i := 0; i < size; i++ {
			storage[i] = q.storage[q.start+i]
		}

		q.storage = storage
		q.start = 0
		q.end = size
	}
	q.storage[q.end] = bin
	q.end++
}

// Pop extracts a value from the back of the queue.
func (q *BinQueue) Pop() Bin {
	q.start++
	return q.storage[q.start-1]
}

// Contents returns a slice containing all values in the queue.
func (q *BinQueue) Contents() []Bin {
	return q.storage[q.start:q.end]
}
