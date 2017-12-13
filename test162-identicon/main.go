package main

import (
	"crypto/sha256"
	"fmt"
	"image"
	"image/png"
	"os"

	"github.com/fogleman/gg"
	"github.com/nfnt/resize"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

func run() error {
	if len(os.Args) != 3 {
		return fmt.Errorf("usage: %s <seed> <filename>", os.Args[0])
	}
	seed := os.Args[1]
	filename := os.Args[2]

	img, err := makeIdenticon(seed)
	if err != nil {
		return err
	}
	return saveScaledPNG(filename, img, 10)
}

// makeIdenticon creates an 8x8 image from given seed.
func makeIdenticon(seed string) (image.Image, error) {
	// We have 64 pixels.
	dc := gg.NewContext(8, 8)

	// Map 256 bits (32 bytes) hash to 32 pixels and create a symmetric image.
	hash := sha256.Sum256([]byte(seed))
	for i, pix := range hash {
		hn := pix % 16
		sn := pix / 16 % 4
		vn := pix / 16 / 4
		h := float64(hn) / 15
		s := float64(sn) / 3
		v := float64(vn) / 3

		// Avoid extreme color.
		s = 0.3 + 0.5*s
		v = 0.4 + 0.6*v

		r, g, b := hsvToRGB(h, s, v)
		dc.SetRGB(r, g, b)

		x := i / 8
		y := i % 8
		dc.SetPixel(x, y)
		dc.SetPixel(7-x, y)
	}

	return dc.Image(), nil
}

// saveScaledPNG saves an image to a file with scaling.
func saveScaledPNG(path string, img image.Image, scale float64) error {
	size := img.Bounds().Size()
	width := uint(float64(size.X) * scale)
	height := uint(float64(size.Y) * scale)
	rimg := resize.Resize(width, height, img, resize.NearestNeighbor)

	file, err := os.Create(path)
	if err != nil {
		return err
	}
	defer file.Close()

	return png.Encode(file, rimg)
}
