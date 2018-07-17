PRODUCTS =       \
  listen         \
  listen-freebsd \
  send           \
  send-freebsd

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

listen: listen.go
	go build -o $@ listen.go

send: send.go
	go build -o $@ send.go

listen-freebsd: listen.go
	GOOS=freebsd go build -o $@ listen.go

send-freebsd: send.go
	GOOS=freebsd go build -o $@ send.go
