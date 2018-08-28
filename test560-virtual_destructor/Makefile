PRODUCT = main

OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++14        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
