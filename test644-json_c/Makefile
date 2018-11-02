OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/include \
  -isystem /usr/local/include

CFLAGS = \
  -std=c99 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

LDFLAGS = \
  -ljson-c

.PHONY: all clean

all: main
	@:

clean:
	rm -f main
