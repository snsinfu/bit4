ARTIFACTS = rtproxy rtagent

.PHONY: all clean

all: $(ARTIFACTS)
	@:

clean:
	rm -f $(ARTIFACTS)

rtproxy: proxy/*
	go build -o $@ ./proxy

rtagent: agent/*
	go build -o $@ ./agent
