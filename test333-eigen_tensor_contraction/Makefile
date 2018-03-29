INCLUDES =                           \
  -isystem /usr/local/include/eigen3 \
  -isystem /usr/local/include/eigen3/unsupported

CXXFLAGS =     \
  -O2          \
  -std=c++14   \
  -pedantic    \
  -Wall        \
  -Wextra      \
  -Wconversion \
  -Wshadow     \
  $(INCLUDES)

.PHONY: run clean

run: main
	./main

clean:
	rm -f main

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
