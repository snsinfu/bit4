INCLUDES = \
  -isystem /usr/local/include

DBGFLAGS =           \
  -fsanitize=address \
  -g

OPTFLAGS = \
  -flto    \
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

.PHONY: run clean

run: main
	./main

clean:
	rm -fr main main.dSYM

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main.cc: math.hpp
