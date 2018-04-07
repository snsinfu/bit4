INCLUDES = \
  -isystem /usr/local/include

DBGFLAGS = \
  -g       \
  -fsanitize=address

OPTFLAGS = \
  -O1

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

.SUFFIXES: .cc
.PHONY: run clean

run: main
	./main

clean:
	rm -f main

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main.cc: beta.hpp expr.hpp
beta.hpp: expr.hpp
