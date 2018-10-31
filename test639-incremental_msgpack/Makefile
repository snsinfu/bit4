OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/include \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

LDFLAGS = \
  -lboost_iostreams

.PHONY: all clean

all: main
	@:

clean:
	rm -f main trajectory.pack.gz
