package main

import (
	"fmt"
	"image/png"
	"os"

	"github.com/fogleman/gg"
	"github.com/nfnt/resize"
)

func main() {
	dc := gg.NewContext(50, 50)
	dc.SetRGB(1, 1, 1)
	dc.Clear()

	dc.DrawRectangle(5.5, 5.5, 40, 40)
	dc.SetRGB(0, 0, 0)
	dc.SetLineWidth(1)
	dc.Stroke()

	r := 123
	g := 231
	b := 213
	for x := 10; x <= 40; x++ {
		for y := 10; y <= 40; y++ {
			dc.SetRGB255(r, g, b)
			dc.SetPixel(x, y)

			r ^= x * 6
			g ^= y * 6
			b ^= g
			g ^= r
			r ^= b

			b = (r * g) % 256
			g = (b * r) % 256
			r = (g * b) % 256
		}
	}

	img := resize.Resize(500, 500, dc.Image(), resize.NearestNeighbor)

	file, err := os.Create("a.png")
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	defer file.Close()

	if err := png.Encode(file, img); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
