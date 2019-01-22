INCLUDES = \
  -isystem include \
  -isystem /usr/local/include

LIBS = \
  -lsqlite3 \
  -lz

OPTFLAGS = \
  -O2 \
  -march=native

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(INCLUDES) \
  $(OPTFLAGS)

LDFLAGS = \
  $(LIBS)

.PHONY: all clean

all: main
	@:

clean:
	rm -f main

main: src/*
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ src/main.cc
