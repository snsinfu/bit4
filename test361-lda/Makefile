INCLUDES = \
  -isystem /usr/local/include

DBGFLAGS = \
  -g

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

OBJECTS = \
  lda.o   \
  main.o

.SUFFIXES: .cc
.PHONY: run clean

run: main
	./main

clean:
	rm -f main $(OBJECTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.cc: lda.hpp
lda.cc: lda.hpp ext/copy.hpp ext/xindex.hpp ext/xmath.hpp
ext/xmath.hpp: ext/copy.hpp ext/math.hpp ext/xindex.hpp ext/xutility.hpp
