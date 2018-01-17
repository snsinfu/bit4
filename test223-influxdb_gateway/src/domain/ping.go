package domain

import (
	"time"
)

type Pong struct {
	Latency time.Duration
	Version string
}

func (dom *Domain) Ping() (Pong, error) {
	lat, ver, err := dom.influx.Ping(0)
	return Pong{lat, ver}, err
}
