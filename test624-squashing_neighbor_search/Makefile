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
  bruteforce_neighbor_searcher.hpp \
  grid_neighbor_searcher.hpp \
  hashing_neighbor_searcher.hpp \
  squashing_neighbor_searcher.hpp \
  finehashing_neighbor_searcher.hpp \
  overlapped_hashing_neighbor_searcher.hpp \
  point.hpp \
  types.hpp
