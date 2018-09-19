package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	src := "sample.dat"
	binSize := int64(1000)

	file, err := os.Open(src)
	if err != nil {
		return err
	}
	defer file.Close()

	return rebed(file, binSize)
}

func rebed(src io.Reader, binSize int64) error {
	s := bufio.NewScanner(src)

	track := newTrack(binSize)
	chrom := ""

	printBin := func(beg, end int64, val float64) {
		fmt.Printf("%s\t%d\t%d\t%g\n", chrom, beg, end, val)
	}

	for s.Scan() {
		fields := strings.Fields(s.Text())

		if len(fields) != 4 {
			continue
		}

		if fields[0] != chrom {
			track.serialize(printBin)
			track = newTrack(binSize)
			chrom = fields[0]
		}

		beg, err := strconv.ParseInt(fields[1], 10, 64)
		if err != nil {
			return err
		}

		end, err := strconv.ParseInt(fields[2], 10, 64)
		if err != nil {
			return err
		}

		val, err := strconv.ParseFloat(fields[3], 64)
		if err != nil {
			return err
		}

		track.put(beg, end, val)
	}

	if err := s.Err(); err != nil {
		return err
	}

	track.serialize(printBin)

	return nil
}

type track struct {
	bins    []float64
	binSize int64
	end     int64
}

func newTrack(binSize int64) *track {
	return &track{binSize: binSize}
}

func (t *track) serialize(callback func(int64, int64, float64)) {
	for beg := int64(0); beg < t.end; beg += t.binSize {
		end := beg + t.binSize
		if end > t.end {
			end = t.end
		}

		i := int(beg / t.binSize)
		mean := t.bins[i] / float64(end-beg)

		callback(beg, end, mean)
	}
}

func (t *track) put(beg, end int64, val float64) {
	bound := int((end + t.binSize - 1) / t.binSize)
	if bound > len(t.bins) {
		newBins := make([]float64, bound + len(t.bins))
		copy(newBins, t.bins)
		t.bins = newBins
	}

	b := beg
	e := (beg/t.binSize + 1) * t.binSize

	for b < end {
		if e > end {
			e = end
		}

		i := int(b / t.binSize)
		t.bins[i] += val * float64(e-b)

		b = e
		e += t.binSize
	}

	if end > t.end {
		t.end = end
	}
}
