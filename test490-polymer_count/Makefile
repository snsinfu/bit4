OPTFLAGS = \
  -O2

CFLAGS =       \
  -std=c11     \
  -pedantic    \
  -Wall        \
  -Wextra      \
  -Wconversion \
  -Wshadow     \
  $(OPTFLAGS)

PRODUCTS = main main_2d

.PHONY: all clean

all: $(PRODUCTS)
	@:

main: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

main_2d: main_2d.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(PRODUCTS)
