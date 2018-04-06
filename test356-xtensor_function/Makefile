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

OBJECTS = \
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

main.cc: beta.hpp
