INCLUDES = \
  -isystem ../deps/micromd/include

DBGFLAGS = \
  -g

OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++2a \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wno-c99-extensions \
  $(INCLUDES) \
  $(DBGFLAGS) \
  $(OPTFLAGS)


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
