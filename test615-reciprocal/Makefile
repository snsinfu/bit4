WARNINGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow

OPTFLAGS = \
  -O2 \
  -msse4 \
  -mno-avx \
  -march=ivybridge \
  -funsafe-math-optimizations \
# -ffinite-math-only

CXXFLAGS = \
  $(WARNINGS) \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(EXTRA_CXXFLAGS)

PRODUCTS = \
  main \
  main.s


.PHONY: all clean
.SUFFIXES: .cc .s

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

.cc.s:
	$(CXX) $(CXXFLAGS) -S -o $@ $<
