OPTFLAGS = \
  -O2 \
  -march=native \
  -mno-avx

DBGFLAGS = \
  -g

INCLUDES = \
  -isystem ../include

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(INCLUDES)

LDFLAGS = \
  -lprofiler

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc

main: array_view.hpp nsearch.hpp point.hpp
