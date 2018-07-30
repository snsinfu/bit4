PRODUCTS = main

OPTFLAGS = \
  -O2

CFLAGS =            \
  -std=c11          \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCTS)
	@$:

clean:
	rm -f $(PRODUCTS)

main: main.c
