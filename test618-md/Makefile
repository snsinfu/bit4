PRODUCTS = \
  example

OPTFLAGS = \
  -O2 \
  -funsafe-math-optimizations

DBGFLAGS = \
  -g

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(EXTRA_CXXFLAGS)

ARTIFACTS = \
  $(PRODUCTS) \
  *.dSYM


.PHONY: all clean
.SUFFIXES: .cc

all: $(PRODUCTS)
	@:

clean:
	rm -rf $(ARTIFACTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

example: md.hpp
