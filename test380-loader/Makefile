SOURCES = serve.go
TARGET = ./serve

.PHONY: run clean

run: $(TARGET)
	$(TARGET)

clean:
	rm -f $(TARGET)

$(TARGET): $(SOURCES)
	go build -o $@
