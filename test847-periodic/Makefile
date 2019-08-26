CXX = clang++-brew

PRODUCTS = \
  main

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g

INCLUDES = \
  -isystem include

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wno-c99-extensions \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(INCLUDES)

ARTIFACTS = \
  $(PRODUCTS)


.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(ARTIFACTS)
