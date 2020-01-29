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


.PHONY: run clean

run: main
	./main
	h5ls -rlv _test.h5

clean:
	rm -f main _test.h5
