PRODUCTS =     \
  listen       \
  listen-linux \
  send         \
  send-linux

.PHONY: all clean deps

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

listen: listen.go
	go build -o $@ listen.go

send: send.go
	go build -o $@ send.go

listen-linux: listen.go
	GOOS=linux go build -o $@ listen.go

send-linux: send.go
	GOOS=linux go build -o $@ send.go
