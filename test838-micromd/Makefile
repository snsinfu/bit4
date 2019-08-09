INCLUDES = \
  -isystem ../include

OPTFLAGS = \
  -O2 \
  -march=x86-64 \
  -mtune=znver2

CXXFLAGS = \
  -std=c++2a \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wno-c99-extensions \
  $(INCLUDES) \
  $(OPTFLAGS)

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
	$(CXX) $(CXXFLAGS) -masm=intel -S -o- $< | c++filt | egrep -v '^[[:space:]]*\.[^L]' > $@
