PRODUCTS = \
  main

WARNINGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow

OPTFLAGS = \
  -O2

CXXFLAGS = \
  $(WARNINGS) \
  $(OPTFLAGS) \
  $(EXTRA_CXXFLAGS)

ARTIFACTS = \
  $(PRODUCTS)

.PHONY: all clean
.SUFFIXES: .cc

all: $(PRODUCTS)
	@:

clean:
	rm -rf $(ARTIFACTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
