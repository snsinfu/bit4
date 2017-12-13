package main

import (
	"math"
)

// hsvToRGB computes RGB values for an HSV triple.
func hsvToRGB(h, s, v float64) (r, g, b float64) {
	p := rem(h, 1) * 3
	q := rem(p, 1) * 2

	x := 1.0
	y := 1.0
	z := 1.0 - s
	if q > 1 {
		x -= s * (q - 1)
	} else {
		y -= s * (1 - q)
	}

	r, g, b = rotate3(x, y, z, -int(p))
	r *= v
	g *= v
	b *= v

	return
}

// rem calculates the euclidean remainder of x divided by y.
func rem(x, y float64) float64 {
	r := math.Mod(x, y)
	if r < 0 {
		return r + y
	}
	return r
}

// rotate3 rotates three numbers (x, y, z) to the left n times.
func rotate3(x, y, z float64, n int) (u, v, w float64) {
	if n > 0 {
		return rotate3(y, z, x, n-1)
	}
	if n < 0 {
		return rotate3(z, x, y, n+1)
	}
	return x, y, z
}
