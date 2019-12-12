CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  $$(pkg-config --cflags eigen3 | sed "s/-I/-isystem/g")


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
