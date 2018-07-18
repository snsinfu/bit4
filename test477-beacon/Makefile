PRODUCTS = beacon beacon-server

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

beacon: client/client.go client/cmd/main.go
	go build -o $@ ./client/cmd

beacon-server: server/server.go server/cmd/main.go
	go build -o $@ ./server/cmd
