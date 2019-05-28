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


.PHONY: all clean

all: example.h5
	h5ls -lrv example.h5

clean:
	rm -f $(ARTIFACTS)

example.h5: main
	./main
