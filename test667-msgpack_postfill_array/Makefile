OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

LDFLAGS = \
  -lmsgpackc

ARTIFACTS = \
  main \
  example.msgpack

.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)
