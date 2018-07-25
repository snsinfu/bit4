OPTFLAGS = \
  -O3

CFLAGS =       \
  -std=c11     \
  -pedantic    \
  -Wall        \
  -Wextra      \
  -Wconversion \
  -Wshadow     \
  $(OPTFLAGS)

PRODUCTS = count2d count3d

.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)
