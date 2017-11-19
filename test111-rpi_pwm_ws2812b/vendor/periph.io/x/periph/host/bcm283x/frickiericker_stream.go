package bcm283x

import (
	"errors"
	"math"
	"time"
	"unsafe"

	"periph.io/x/periph/conn/gpio/gpiostream"
	"periph.io/x/periph/host/pmem"
	"periph.io/x/periph/host/videocore"
)

func (p *Pin) StreamOut(s gpiostream.Stream) error {
	if p.number != 18 {
		return p.wrap(errors.New("StramOut is implemented only for GPIO18"))
	}
	if dmaMemory == nil || pwmMemory == nil || clockMemory == nil {
		return p.wrap(errors.New("bcm283x-dma not initialized; try again as root?"))
	}

	bs, ok := s.(*gpiostream.BitStream)
	if !ok {
		return p.wrap(errors.New("StreamOut is currently implemented only for BitStream"))
	}

	const (
		pageSize = 4096
		pwmOffset = 0x20c000
	)

	// Set up the source buffer.
	pSrc, err := videocore.Alloc((len(bs.Bits) + pageSize - 1) / pageSize * pageSize)
	if err != nil {
		return p.wrap(err)
	}
	defer pSrc.Close()

	copy(pSrc.Bytes(), bs.Bits)

	// Set up the control block.
	pCB, err := videocore.Alloc(pageSize)
	if err != nil {
		return p.wrap(err)
	}
	defer pCB.Close()

	var cb *controlBlock
	if err := pCB.AsPOD(&cb); err != nil {
		return p.wrap(err)
	}
	srcAddr := uint32(pSrc.PhysAddr())
	destAddr := baseAddr + pwmOffset + uint32(unsafe.Offsetof(pwmMemory.fifo))
	if err := cb.initBlock(srcAddr, destAddr, uint32(len(bs.Bits)), false, true, dmaPWM, 0); err != nil {
		return err
	}

	// Set the clock.
	clk, div, hz, err := calcApproxTimingSource(bs.Res)
	if err != nil {
		return p.wrap(err)
	}
	if err := clockMemory.pwm.setRaw(clk, div); err != nil {
		return p.wrap(err)
	}

	// Run PWM.
	if err := enableDMAPWM(7, 3); err != nil {
		return p.wrap(err)
	}
	if err := runBlockingIO(pCB, len(bs.Bits), hz); err != nil {
		return p.wrap(err)
	}

	return nil
}

// runBlockingIO picks a DMA channel, initialize it and runs a transfer.
func runBlockingIO(pCB pmem.Mem, bytes int, hz uint64) error {
	blacklist := []int{7, 8, 9, 10, 11, 12, 13, 14, 15}

	_, ch := pickChannel(blacklist...)
	if ch == nil {
		return errors.New("bcm283x-dma: no channel available")
	}
	defer ch.reset()

	cbAddr := physToUncachedPhys(uint32(pCB.PhysAddr()))
	ch.startIO(cbAddr)

	return ch.waitTransfer(bytes, hz)
}

// waitTransfer waits for a DMA channel transmission to complete.
func (ch *dmaChannel) waitTransfer(size int, hz uint64) error {
	txTime := float64(size * 8) / float64(hz)
	<-time.NewTimer(time.Duration(txTime * float64(time.Second))).C
	return ch.wait()
}

// enableDMAPWM sets PWM-1 to 32-bit serialiser mode via DMA.
func enableDMAPWM(panik, dreq int) error {
	pwmMemory.rng1 = 32
	Nanospin(10 * time.Microsecond)
	pwmMemory.dmaCfg = pwmDMAEnable | pwmDMACfg(panik << pwmPanicShift | dreq)
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl = pwmClearFIFO
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl = pwm1UseFIFO | pwm1Serialiser
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl |= pwm1Enable
	Nanospin(10 * time.Microsecond)

	if pwmMemory.ctl & pwm1Enable == 0 {
		return errors.New("failed to enable PWM")
	}
	return nil
}

// calcApproxTimingSource choose the best source to get a clock that
// approximately has the desired period.
func calcApproxTimingSource(period time.Duration) (clockCtl, int, uint64, error) {
	div1, hz1, actual1 := calcApproxTimingDivisor(clk19dot2MHz, period)
	div2, hz2, actual2 := calcApproxTimingDivisor(clk500MHz, period)

	delta1 := math.Abs(actual1 - float64(period))
	delta2 := math.Abs(actual2 - float64(period))

	if div1 != 0 && delta1 < delta2 {
		return clockSrc19dot2MHz, div1, hz1, nil
	}
	if div2 != 0 {
		return clockSrcPLLD, div2, hz2, nil
	}
	return 0, 0, 0, errors.New("failed to find a good clock")
}

// calcApproxTimingDivisor tries to find the clock divisor to achieve the
// desired period.
//
// The clock divisor is capped to clockDiviMax.
func calcApproxTimingDivisor(srcHz uint64, period time.Duration) (int, uint64, float64) {
	const scale = uint64(time.Second)

	div := (srcHz * uint64(period) + scale / 2) / scale
	if div > clockDiviMax {
		return 0, 0, math.NaN()
	}
	hz := srcHz / div
	actual := float64(scale) / float64(hz)

	return int(div), hz, actual
}
