package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	imageName := "test.img"
	diskSize := 1024 * 1024 * 16
	partType := uint8(0x0c) // FAT32 LBA

	return createDisk(imageName, diskSize, partType)
}

func createDisk(name string, size int, partType uint8) error {
	img, err := os.Create(name)
	if err != nil {
		return err
	}
	defer img.Close()

	buf := bufio.NewWriter(img)

	sectorCount := size / SectorSize
	writeMBR(buf, sectorCount, partType)

	sector := make([]byte, SectorSize)
	for i := 1; i < sectorCount; i++ {
		buf.Write(sector[:])
	}

	return buf.Flush()
}

// writeMBR writes MBR with single partition configuration.
// https://en.wikipedia.org/wiki/Master_boot_record
func writeMBR(wr *bufio.Writer, sectorCount int, partType uint8) {
	signature := uint32(0x12345678)
	protection := uint16(0x0000)

	wr.Write(make([]byte, 218)) // Bootcode part 1
	wr.Write(make([]byte, 6))   // Timestamp
	wr.Write(make([]byte, 216)) // Bootcode part 2
	write4(wr, signature)
	write2(wr, protection)

	writePE(wr, PartitionEntry{
		Bootable: false,
		First:    Address{1},
		Last:     Address{uint32(sectorCount - 1)},
		Type:     partType,
	})
	writePE(wr, PartitionEntry{})
	writePE(wr, PartitionEntry{})
	writePE(wr, PartitionEntry{})

	// Boot signature
	write1(wr, 0x55)
	write1(wr, 0xAA)
}

type PartitionEntry struct {
	Bootable bool
	First    Address
	Last     Address
	Type     uint8
}

func writePE(wr *bufio.Writer, p PartitionEntry) {
	if p.Bootable {
		write1(wr, 0x80)
	} else {
		write1(wr, 0x00)
	}
	writeCHS(wr, p.First)
	write1(wr, p.Type)
	writeCHS(wr, p.Last)
	write4(wr, p.First.LBA)
	write4(wr, p.Last.LBA - p.First.LBA + 1)
}

type Address struct {
	LBA uint32
}

const (
	HeadsPerCylinder = 16
	SectorsPerTrack  = 63
	SectorSize       = 512
)

func (a *Address) Head() uint8 {
	return uint8(a.LBA / SectorsPerTrack % HeadsPerCylinder)
}

func (a *Address) Cylinder() uint16 {
	return uint16(a.LBA / (SectorsPerTrack * HeadsPerCylinder))
}

func (a *Address) Sector() uint8 {
	return uint8(a.LBA % SectorsPerTrack + 1)
}

func writeCHS(wr *bufio.Writer, a Address) {
	c := a.Cylinder()
	h := a.Head()
	s := a.Sector()

	write1(wr, h)
	write1(wr, byte(s | uint8((c >> 2) & 0xc0)))
	write1(wr, byte(c))
}

func write1(wr *bufio.Writer, value uint8) {
	wr.WriteByte(value)
}

func write2(wr *bufio.Writer, value uint16) {
	wr.WriteByte(byte(value))
	wr.WriteByte(byte(value >> 8))
}

func write4(wr *bufio.Writer, value uint32) {
	wr.WriteByte(byte(value))
	wr.WriteByte(byte(value >> 8))
	wr.WriteByte(byte(value >> 16))
	wr.WriteByte(byte(value >> 24))
}
