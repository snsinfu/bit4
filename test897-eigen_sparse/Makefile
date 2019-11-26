INCLUDES = \
  -isystem ../deps/BlueBrain/HighFive/include \
  -isystem /home/linuxbrew/.linuxbrew/include/eigen3

OPTFLAGS = \
  -O2 \
  -march=native

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -g \
  $(INCLUDES) \
  $(OPTFLAGS)

LDFLAGS = \
  -lprofiler


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
