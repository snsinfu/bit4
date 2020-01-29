CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2

LDFLAGS = \
  -lhdf5

OBJECTS = \
  main.o \
  h5_store.o

ARTIFACTS = \
  $(OBJECTS) \
  main \
  _test.h5


.PHONY: run clean

run: main
	./main
	h5ls -rlv _test.h5

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

main.o: h5_store.hpp
h5_store.o: h5_store.hpp
