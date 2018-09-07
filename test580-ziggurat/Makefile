OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

PRODUCT = main

.PHONY: test clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

$(PRODUCT): main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
