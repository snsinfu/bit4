package bcm283x

import (
	"errors"
	"fmt"
	"time"
	"unsafe"

	"periph.io/x/periph/conn/gpio/gpiostream"
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
		return errors.New("StreamOut() is implemented only for BitStream")
	}

	//
	pageSize := 4096
	pCB, err := videocore.Alloc(pageSize)
	if err != nil {
		return err
	}
	defer pCB.Close()

	fmt.Printf("pCB = %v", pCB)

	var cb *controlBlock
	if err := pCB.AsPOD(&cb); err != nil {
		return err
	}
	fmt.Printf("&cb = %v\n", unsafe.Pointer(cb))

	//
	pSrc, err := videocore.Alloc((len(bs.Bits) + pageSize - 1) / pageSize * pageSize)
	if err != nil {
		return err
	}
	defer pSrc.Close()

	fmt.Printf("pSrc = %v", pSrc)

	src := pSrc.Bytes()[:len(bs.Bits)]
	copy(src, bs.Bits)

	fmt.Printf("src = %v\n", src)

	srcAddr := uint32(pSrc.PhysAddr())
	pwmAddr := baseAddr + 0x20c000
	destAddr := pwmAddr + uint32(unsafe.Offsetof(pwmMemory.fifo))

	fmt.Printf("baseAddr = %x\n", baseAddr)
	fmt.Printf("dmaMemory = %p\n", unsafe.Pointer(dmaMemory))
	fmt.Printf("pwmMemory = %p\n", unsafe.Pointer(pwmMemory))
	fmt.Printf("clockMemory = %p\n", unsafe.Pointer(clockMemory))

	fmt.Printf("phys CB = %x\n", pCB.PhysAddr())
	fmt.Printf("src addr = %x\n", srcAddr)
	fmt.Printf("dest addr = %x\n", destAddr)

	//
	hz := uint64(time.Second / bs.Res)
	actual, div, err := clockMemory.pwm.set(hz, dmaWaitcyclesMax + 1)
	if err != nil {
		return err
	}

	fmt.Printf("hz = %v, actual = %v, div = %v\n", hz, actual, div)
	fmt.Printf("clock = %v\n", clockMemory)

	if err := cb.initBlock(srcAddr, destAddr, uint32(len(bs.Bits)), false, true, dmaPWM, 0); err != nil {
		return err
	}

	//
	pwmMemory.rng1 = 32
	Nanospin(10 * time.Microsecond)
	pwmMemory.dmaCfg = pwmDMAEnable | pwmDMACfg(7 << pwmPanicShift | 3)
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl = pwmClearFIFO
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl = pwm1UseFIFO | pwm1Serialiser
	Nanospin(10 * time.Microsecond)
	pwmMemory.ctl |= pwm1Enable

	fmt.Printf("pwm = %v\n", pwmMemory)
	fmt.Printf("PWEN = %v\n", pwmMemory.ctl & pwm1Enable)
	fmt.Printf("pwm rng1 = %v\n", pwmMemory.rng1)

	//
	//_, ch := pickChannel()
	//if ch == nil {
	//	return errors.New("bcm283x-dma: no channel available")
	//}
	ch := &dmaMemory.channels[5]
	ch.reset()

	//ch.cs = dmaReset
	//Nanospin(10 * time.Microsecond)
	//ch.cs = dmaEnd | dmaInterrupt
	//Nanospin(10 * time.Microsecond)
	//ch.debug = dmaReadError | dmaFIFOError | dmaReadLastNotSetError
	//ch.cbAddr = uint32(pCB.PhysAddr())
	//cbBus, err := pCB.Bus()
	//if err != nil {
	//	return err
	//}
	ch.cbAddr = physToUncachedPhys(uint32(pCB.PhysAddr()))
	ch.cs = dmaWaitForOutstandingWrites | dmaStatus(15 << dmaPanicPriorityShift) | dmaStatus(15 << dmaPriorityShift) | dmaActive
	Nanospin(10 * time.Microsecond)

	//fmt.Printf("bus = %x\n", cbBus)
	fmt.Printf("ch = %v\n", ch)
	fmt.Printf("active = %v\n", ch.cs & dmaActive)
	fmt.Printf("debug = %v\n", ch.debug)

	txTime := time.Duration(len(bs.Bits) * 8) * bs.Res
	fmt.Printf("wait %v\n", txTime)
	<-time.NewTimer(txTime).C

	return nil
}
