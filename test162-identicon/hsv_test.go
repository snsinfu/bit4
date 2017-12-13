package main

import (
	"math"
	"testing"
)

func Test_hsvToRGB(t *testing.T) {
	testCases := []struct {
		h, s, v float64
		r, g, b float64
	}{
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // black
		{0.0, 0.0, 0.5, 0.5, 0.5, 0.5}, // gray
		{0.0, 0.0, 1.0, 1.0, 1.0, 1.0}, // white

		{0.0, 1.0, 1.0, 1.0, 0.0, 0.0},  // red
		{1.0, 1.0, 1.0, 1.0, 0.0, 0.0},  // red (hue 1)
		{2.0, 1.0, 1.0, 1.0, 0.0, 0.0},  // red (hue 2)
		{-1.0, 1.0, 1.0, 1.0, 0.0, 0.0}, // red (hue -1)

		{0.1666667, 1.0, 1.0, 1.0, 1.0, 0.0}, // yellow
		{0.3333333, 1.0, 1.0, 0.0, 1.0, 0.0}, // green
		{0.5000000, 1.0, 1.0, 0.0, 1.0, 1.0}, // cyan
		{0.6666667, 1.0, 1.0, 0.0, 0.0, 1.0}, // blue
		{0.8333333, 1.0, 1.0, 1.0, 0.0, 1.0}, // magenta

		{0.0000000, 0.5, 1.0, 1.0, 0.5, 0.5}, // pale red
		{0.1666667, 0.5, 1.0, 1.0, 1.0, 0.5}, // pale yellow
		{0.3333333, 0.5, 1.0, 0.5, 1.0, 0.5}, // pale green
		{0.5000000, 0.5, 1.0, 0.5, 1.0, 1.0}, // pale cyan
		{0.6666667, 0.5, 1.0, 0.5, 0.5, 1.0}, // pale blue
		{0.8333333, 0.5, 1.0, 1.0, 0.5, 1.0}, // pale magenta

		{0.0000000, 1.0, 0.5, 0.5, 0.0, 0.0}, // dark red
		{0.1666667, 1.0, 0.5, 0.5, 0.5, 0.0}, // dark yellow
		{0.3333333, 1.0, 0.5, 0.0, 0.5, 0.0}, // dark green
		{0.5000000, 1.0, 0.5, 0.0, 0.5, 0.5}, // dark cyan
		{0.6666667, 1.0, 0.5, 0.0, 0.0, 0.5}, // dark blue
		{0.8333333, 1.0, 0.5, 0.5, 0.0, 0.5}, // dark magenta
	}
	const tol = 1e-6

	for _, c := range testCases {
		r, g, b := hsvToRGB(c.h, c.s, c.v)

		if math.Abs(r-c.r) > tol || math.Abs(g-c.g) > tol || math.Abs(b-c.b) > tol {
			t.Errorf(
				"(%v, %v, %v) -> (%v, %v, %v), expected (%v, %v, %v)",
				c.h, c.s, c.v, r, g, b, c.r, c.g, c.b,
			)
		}
	}
}

func Test_rem(t *testing.T) {
	testCases := []struct {
		x, y, m float64
	}{
		{0.0, 1.0, 0.0},
		{0.2, 1.0, 0.2},
		{0.5, 1.0, 0.5},
		{0.7, 1.0, 0.7},
		{1.0, 1.0, 0.0},
		{1.2, 1.0, 0.2},
		{1.5, 1.0, 0.5},
		{1.7, 1.0, 0.7},
		{-1.0, 1.0, 0.0},
		{-0.8, 1.0, 0.2},
		{-0.5, 1.0, 0.5},
		{-0.3, 1.0, 0.7},
	}
	const tol = 1e-6

	for _, c := range testCases {
		m := rem(c.x, c.y)

		if m < 0 {
			t.Errorf("(%v, %v) -> %v, expected positive", c.x, c.y, m)
		}
		if math.Abs(m-c.m) > tol {
			t.Errorf("(%v, %v) -> %v, expected %v", c.x, c.y, m, c.m)
		}
	}
}

func Test_rotate3(t *testing.T) {
	testCases := []struct {
		x, y, z float64
		n       int
		u, v, w float64
	}{
		{1, 2, 3, 0, 1, 2, 3},
		{1, 2, 3, 1, 2, 3, 1},
		{1, 2, 3, 2, 3, 1, 2},
		{1, 2, 3, 3, 1, 2, 3},
		{1, 2, 3, -1, 3, 1, 2},
		{1, 2, 3, -2, 2, 3, 1},
		{1, 2, 3, -3, 1, 2, 3},
	}

	for _, c := range testCases {
		u, v, w := rotate3(c.x, c.y, c.z, c.n)

		if u != c.u || v != c.v || w != c.w {
			t.Errorf(
				"(%v, %v, %v, %v) -> (%v, %v, %v), expected (%v, %v, %v)",
				c.x, c.y, c.z, c.n,
				u, v, w,
				c.u, c.v, c.w,
			)
		}
	}
}
