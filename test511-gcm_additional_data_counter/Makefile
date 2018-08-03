PRODUCT = main
ARTIFACTS = $(PRODUCT) depends.ok

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(ARTIFACTS)

depends.ok:
	go get .
	@touch $@

main: main.go depends.ok
	go build -o $@ main.go
