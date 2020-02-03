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


.PHONY: check clean

check: test.h5
	@h5ls -rlv $<

clean:
	rm -f $(ARTIFACTS)

test.h5: main
	./main

main: main.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main: unique_hid.hpp
