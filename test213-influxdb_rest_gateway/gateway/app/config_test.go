package app

import (
	"os"
	"testing"
)

func TestGetConfig(t *testing.T) {
	const addr = "influxdb.example.com:1234"

	os.Setenv("GATEWAY_INFLUXDB_ADDRESS", addr)

	config, err := GetConfig()
	if err != nil {
		t.Errorf("unexpected error: %v", err)
	}

	if config.DatabaseAddress != addr {
		t.Errorf(
			"GetConfig() -> got address=%v, expected %v",
			config.DatabaseAddress, addr)
	}
}
