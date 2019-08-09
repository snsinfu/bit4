CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -O2

PRODUCTS = \
  main \
  main.s

.PHONY: all clean
.SUFFIXES: .s

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.cc.s:
	$(CXX) $(CXXFLAGS) -masm=intel -S -o- $< | c++filt | egrep -v '^[[:space:]]*\.' > $@
