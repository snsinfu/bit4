package domain

import (
	"github.com/influxdata/influxdb/client/v2"
)

type Domain struct {
	influx client.Client
}

func New(influx client.Client) *Domain {
	return &Domain{influx}
}
