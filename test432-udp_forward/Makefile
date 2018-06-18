TARGETS = proxy client server

.SUFFIXES: .go
.PHONY: run clean

run: $(TARGETS)
	@:

clean:
	rm -f $(TARGETS)

.go:
	go build -o $@ $<
