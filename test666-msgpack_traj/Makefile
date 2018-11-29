OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++2a \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

LDFLAGS = \
  -lboost_iostreams \
  -lmsgpackc

ARTIFACTS = \
  main \
  trajectory.msgpack.gz

.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)
