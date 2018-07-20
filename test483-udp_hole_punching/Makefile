PRODUCTS = puncher puncher-server

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

puncher: client/cmd/* client/*
	go build -o $@ ./client/cmd

puncher-server: server/cmd/* server/*
	go build -o $@ ./server/cmd
