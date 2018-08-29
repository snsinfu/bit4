PRODUCT = main

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g       \
  -fsanitize=address

INCLUDES = \
  -I ../test510-geo

CXXFLAGS =            \
  -std=c++14          \
  -pedantic           \
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

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
