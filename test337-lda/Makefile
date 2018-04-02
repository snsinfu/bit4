INCLUDES = \
  -isystem /usr/local/include

DBGFLAGS = \
  -DNDEBUG

OPTFLAGS = \
  -flto    \
  -O3

CXXFLAGS =            \
  -std=c++14          \
  -pedantic           \
  -Wall               \
  -Wextra             \
  -Wconversion        \
  -Wshadow            \
  -Wno-missing-braces \
  $(OPTFLAGS)         \
  $(DBGFLAGS)         \
  $(INCLUDES)

.PHONY: run clean

run: main
	./main

clean:
	rm -f main

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)
