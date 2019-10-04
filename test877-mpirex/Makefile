CXX = mpic++

OPTFLAGS = \
  -O2

LIBS = \
  -lmpi

CXXFLAGS = \
  -std=c++17 \
  -Wall \
  -Wextra \
  -Wpedantic \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(OPTFLAGS)

LDFLAGS = \
  $(LIBS)

PRODUCT = \
  main

.PHONY: all clean

all: main
	@:

clean:
	rm -f $(PRODUCT)
