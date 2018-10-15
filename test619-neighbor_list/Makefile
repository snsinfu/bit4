OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g \
  -fsanitize=address

CXXFLAGS = \
  -std=c++14 \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS)

OBJECTS = \
  main.o \
  neighbor_searcher_test.o \
  neighbor_list_test.o


.PHONY: test clean
.SUFFIXES: .cc

test: main
	./main

clean:
	rm -f $(OBJECTS) main *.dSYM

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.o: catch.hpp
neighbor_searcher_test.o: catch.hpp md.hpp
neighbor_list_test.o: catch.hpp md.hpp
