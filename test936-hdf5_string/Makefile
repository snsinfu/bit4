CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(DBGFLAGS) \
  $(OPTFLAGS)

INCLUDES = \
  -I include

DBGFLAGS = \
  -g

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lhdf5

ARTIFACTS = \
  main \
  _test.h5


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main: unique_hid.hpp
