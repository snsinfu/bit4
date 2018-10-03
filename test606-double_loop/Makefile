PRODUCTS = \
  main \
  main.s

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
  -funsafe-math-optimizations

CXXFLAGS = \
  $(WARNINGS) \
  $(OPTFLAGS) \
  $(EXTRA_CXXFLAGS)

.PHONY: all clean
.SUFFIXES: .cc .s

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

.cc.s:
	$(CXX) $(CXXFLAGS) -S -masm=intel -o $@ $<
