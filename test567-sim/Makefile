PRODUCT = main

OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g       \
  -fsanitize=address

INCLUDES = \
  -I .

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

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
