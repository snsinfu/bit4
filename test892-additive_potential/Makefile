CXX = clang++

INCLUDES = \
  -isystem ../deps/micromd/include

OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wno-c99-extensions \
  $(INCLUDES) \
  $(OPTFLAGS)


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
