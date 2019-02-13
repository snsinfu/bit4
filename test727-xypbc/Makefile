OPTFLAGS = \
  -O2 \
  -march=native \
  -mno-avx

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
  $(INCLUDES)

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ main.cc

main: nsearch.hpp
