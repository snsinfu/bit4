CXXFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -O2

ARTIFACTS = \
  main \
  samples.dat

.SUFFIXES: .cc
.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
