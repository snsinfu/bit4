WXLIBS = core,base

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  $$(wx-config --cxxflags | sed "s/-I/-isystem/g")

LDFLAGS = \
  $$(wx-config --libs $(WXLIBS))


.PHONY: all clean

all: main
	@:

clean:
	rm -f main
