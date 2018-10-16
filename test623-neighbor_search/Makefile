OPTFLAGS = \
  -O2

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
  main \
  *.dSYM


.PHONY: all clean
.SUFFIXES: .cc

all: main
	./main

clean:
	rm -rf $(ARTIFACTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

main: \
  array_view.hpp \
  grid_neighbor_searcher.hpp \
  hashing_neighbor_searcher.hpp \
  point.hpp \
  types.hpp
