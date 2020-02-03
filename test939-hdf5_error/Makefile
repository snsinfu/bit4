CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(OPTFLAGS)

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lhdf5

ARTIFACTS = \
  main \
  test.h5


.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main: unique_hid.hpp
