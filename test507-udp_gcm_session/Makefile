PRODUCTS = client server

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

client: client.go
	go build -o $@ client.go

server: server.go
	go build -o $@ server.go
