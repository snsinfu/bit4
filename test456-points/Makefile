PRODUCTS = main

HEADERS = \
  geo/*.hpp

OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++1z        \
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

main: main.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
