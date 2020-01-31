CXXFLAGS = \
  -std=c++17 \
  -stdlib=libc++ \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(INCLUDES) \
  $(OPTFLAGS)

INCLUDES = \
  -I include

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lc++fs \
  -lhdf5 \


.PHONY: all clean

all: main
	@:

clean:
	rm -f main

main: main.cc include/h5.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cc $(LDFLAGS)
