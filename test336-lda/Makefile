INCLUDES =                           \
  -isystem /usr/local/include/eigen3 \
  -isystem /usr/local/include/eigen3/unsupported

OPTFLAGS = \
  -DNDEBUG \
  -O3

CXXFLAGS =     \
  -std=c++14   \
  -pedantic    \
  -Wall        \
  -Wextra      \
  -Wconversion \
  -Wshadow     \
  $(OPTFLAGS)  \
  $(INCLUDES)

.PHONY: run clean

run: main
	./main

clean:
	rm -f main

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
