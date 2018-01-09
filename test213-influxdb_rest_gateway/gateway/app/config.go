package app

import (
	"os"
)

// Config stores run-time parameters for the gateway app.
type Config struct {
	DatabaseAddress string
}

// GetConfig loads parameters from the environment.
func GetConfig() (*Config, error) {
	c := &Config{
		DatabaseAddress: optEnv("GATEWAY_INFLUXDB_ADDRESS", "localhost:8086"),
	}
	return c, nil
}

func optEnv(key, def string) string {
	env := os.Getenv(key)
	if len(env) == 0 {
		return def
	}
	return env
}
