DBGFLAGS = \
  -g

OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem include

override LDFLAGS += \
  -lhdf5 \
  -lz

override CXXFLAGS := \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions \
  $(DBGFLAGS) \
  $(OPTFLAGS) \
  $(INCLUDES) \
  $(CXXFLAGS)

ARTIFACTS = \
  main \
  test.h5


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)
