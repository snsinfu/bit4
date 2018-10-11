PRODUCTS = \
  ex01-packing

WARNINGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g

CXXFLAGS = \
  $(WARNINGS) \
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

ex01-packing: sim.hpp
