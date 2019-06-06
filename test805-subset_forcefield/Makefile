CXX = clang++-brew

DBGFLAGS = \
  -DNDEBUG

OPTFLAGS = \
  -O2 \
  -march=x86-64 \
  -mtune=znver1 \
  -mno-avx \
  -msse4

INCLUDES = \
  -isystem include

DEFINES = \
  -DUSE_ADHOC=0

override CXXFLAGS := \
  -std=c++2a \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions \
  $(DBGFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES) \
  $(DEFINES) \
  $(CXXFLAGS)

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -rf $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

main: Makefile
main: include/md.hpp
