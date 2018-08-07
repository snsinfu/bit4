PRODUCT = main

OPTFLAGS = \
  -O2

ARCHFLAGS = \
  -msse2    \
  -maes

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(ARCHFLAGS)      \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
