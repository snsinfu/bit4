package main

import (
	"fmt"
	"os"

	"gopkg.in/yaml.v2"
)

type Config struct {
	ListenAddress string `yaml:"listen_address"`
	ListenPort    int    `yaml:"listen_port"`
}

var (
	DefaultConfig = Config{
		ListenAddress: "0.0.0.0",
		ListenPort:    3000,
	}
)

func main() {
	data := []byte("listen_address: 127.0.0.1\n")
	config := DefaultConfig

	if err := yaml.Unmarshal(data, &config); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}

	fmt.Println(config)
}
