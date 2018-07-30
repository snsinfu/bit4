PRODUCTS = main

OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++14        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wmost            \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
