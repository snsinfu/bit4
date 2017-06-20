package main

import (
	"fmt"
)

func main() {

	src := []Bin{
		{1, 5, 1.0},
		{7, 9, 2.0},
		{12, 15, 3.0},
		{17, 20, 4.0},
	}

	bins := rebin(src, 2, -5, 22)

	for _, bin := range bins {
		fmt.Printf("%d\t%d\t%g\n", bin.Start, bin.End, bin.Value)
	}
}

// Bin represents an integer interval [Start .. End) annotated with a Value.
type Bin struct {
	Start, End int64
	Value      float64
}

func rebin(input []Bin, binSize, offset, end int64) []Bin {

	span := func(bin Bin) float64 {
		return float64(bin.End - bin.Start)
	}

	normalize := func(bin Bin) Bin {
		if bin.Start < 0 {
			bin.Start = 0
		}
		if bin.End > end {
			bin.End = end
		}
		bin.Value /= span(bin)
		return bin
	}

	advance := func(bin Bin) Bin {
		return Bin{bin.Start + binSize, bin.End + binSize, 0}
	}

	bins := []Bin{}

	send := func(bin Bin) {
		if bin.End <= 0 {
			return
		}
		bins = append(bins, normalize(bin))
	}

	bin := Bin{offset, offset + binSize, 0}

	for _, fragment := range input {

		if fragment.End <= bin.Start {
			continue
		}

		if fragment.Start < bin.Start {
			fragment.Start = bin.Start
		}

		for bin.End <= fragment.Start {
			send(bin)
			bin = advance(bin)
		}

		for bin.End < fragment.End {
			bin.Value += span(bin) * fragment.Value
			send(bin)
			bin = advance(bin)
			fragment.Start = bin.Start
		}

		bin.Value += span(fragment) * fragment.Value
	}

	for bin.Start < end {
		send(bin)
		bin = advance(bin)
	}

	return bins
}
