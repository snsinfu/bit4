OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++2a \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions \
  $(INCLUDES) \
  $(OPTFLAGS)

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

main: math.hpp vec.hpp
