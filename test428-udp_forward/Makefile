TARGETS = proxy client server

SERVER_ADDR = localhost:4000
PROXY_ADDR = localhost:5000

.PHONY: run clean

run: $(TARGETS)
	@:

clean:
	rm -f $(TARGETS)

proxy: proxy.go
	go build -o $@ $<

client: client.go
	go build -o $@ $<

server: server.go
	go build -o $@ $<
