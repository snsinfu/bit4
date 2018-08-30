PRODUCT = main

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g       \
  -fsanitize=address

INCLUDES = \
  -I .     \
  -I cxx   \
  -I geo

CXXFLAGS =            \
  -std=c++14          \
  -pedantic           \
  -Wno-c99-extensions \
  -Wall               \
  -Wextra             \
  -Wconversion        \
  -Wsign-conversion   \
  -Wshadow            \
  $(OPTFLAGS)         \
  $(INCLUDES)

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

main: main.cc cxx/*.hpp geo/*.hpp sim/*.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
