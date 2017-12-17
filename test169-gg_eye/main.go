package main

import (
	"fmt"
	"math/rand"
	"os"

	"github.com/fogleman/gg"
)

func main() {
	dc := gg.NewContext(400, 400)
	dc.SetRGB(1, 1, 1)
	dc.Clear()

	rand.Seed(1010)

	r := 40.0
	for i := 0; i < 6; i++ {
		n := 1 << uint(i)
		w := 60 / float64(i+1)
		r += w / 2
		drawArcs(dc, r, n, w)
		r += w / 2
	}

	if err := dc.SavePNG("eye.png"); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

func drawArcs(dc *gg.Context, r float64, n int, w float64) {
	span := 360 / float64(n)
	gap := -1.0

	for i := 0; i < n; i++ {
		beg := float64(i) * span
		end := beg + span

		beg += gap / 2
		end -= gap / 2

		dc.DrawArc(200, 200, r, gg.Radians(beg), gg.Radians(end))
		dc.SetRGB(rand.Float64(), rand.Float64(), rand.Float64())
		dc.SetLineWidth(w)
		dc.SetLineCapButt()
		dc.Stroke()
	}
}
