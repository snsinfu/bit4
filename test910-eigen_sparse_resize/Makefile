OPTFLAGS = \
  -O2 \
  -march=x86-64 \
  -mtune=haswell \
  -mavx2

DBGFLAGS = \
  -g

PKGFLAGS = \
  $$(pkg-config --cflags eigen3 | sed "s/-I/-isystem/g")

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(OPTFLAGS) \
  $(DBGFLAGS) \
  $(PKGFLAGS)

LDFLAGS = \
  -lprofiler


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
