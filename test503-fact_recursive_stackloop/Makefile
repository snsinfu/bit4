PRODUCTS = main

OPTFLAGS = \
  -O2

CFLAGS =            \
  -std=c11          \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wshadow          \
  -Wsign-conversion \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCTS)
	@$:

clean:
	rm -f $(PRODUCTS)

main: main.c
