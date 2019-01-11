CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -O2 \
  -march=native

.PHONY: run clean

run: main
	perf stat -d ./main

clean:
	rm -f main
