package main

import (
	"fmt"
	"log"

	"gopkg.in/yaml.v2"
)

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	repr, err := yaml.Marshal(map[string]interface{}{
		"ports": []ProtoPort{
			ProtoPort{"tcp", 22},
			ProtoPort{"udp", 60000},
			ProtoPort{"udp", 60001},
		},
	})
	if err != nil {
		return err
	}

	fmt.Println(string(repr))

	data := map[string][]ProtoPort{}
	if err := yaml.Unmarshal(repr, data); err != nil {
		return err
	}

	fmt.Println(data)

	return nil
}
