package main

import (
	"fmt"
)

type ProtoPort struct {
	Protocol string
	Port     int
}

func (p ProtoPort) String() string {
	return fmt.Sprintf("%d/%s", p.Port, p.Protocol)
}

func ParseProtoPort(s string) (ProtoPort, error) {
	p := ProtoPort{}
	_, err := fmt.Sscanf(s, "%d/%s", &p.Port, &p.Protocol)
	return p, err
}

func (p ProtoPort) MarshalYAML() (interface{}, error) {
	return p.String(), nil
}

func (p *ProtoPort) UnmarshalYAML(unmarshal func(interface{}) error) error {
	var repr string

	if err := unmarshal(&repr); err != nil {
		return err
	}

	rp, err := ParseProtoPort(repr)
	if err != nil {
		return err
	}
	p.Protocol = rp.Protocol
	p.Port = rp.Port

	return nil
}
