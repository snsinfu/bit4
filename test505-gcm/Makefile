PRODUCT = main

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

main: main.go
	go build -o $@ main.go
