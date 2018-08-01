PRODUCT = main

OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wmost            \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

main: main.cc geo.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
