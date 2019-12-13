CFLAGS = \
  -std=c99 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2

.PHONY: all clean res virt

all: main
	@:

clean:
	rm -f main

res: main
	ulimit -m 1048576; ./main

virt: main
	ulimit -v 1048576; ./main
