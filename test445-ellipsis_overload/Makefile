CXXFLAGS =          \
  -std=c++14        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  -Wno-c99-extensions

.SUFFIXES: .cc
.PHONY: all clean

all: main
	@:

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
