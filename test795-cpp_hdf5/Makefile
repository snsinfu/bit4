CXX = clang++-brew

DBGFLAGS = \
  -g

OPTFLAGS = \
  -O2 \
  -march=native

INCLUDES = \
  -isystem ../include

LDFLAGS = \
  -lhdf5 \
  -lz

WARNINGS = \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions

CXXFLAGS = \
  -std=c++17 \
  $(WARNINGS) \
  $(DBGFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES)

ARTIFACTS = \
  main \
  example.h5


.PHONY: all clean ls check

all: example.h5
	@:

clean:
	rm -f $(ARTIFACTS)

ls: example.h5
	h5ls -lrv example.h5

check: example.h5
	python load.py example.h5 steps/5

example.h5: main
	./main
