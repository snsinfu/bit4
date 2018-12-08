OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++2a        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: run clean

run: main
	@./main

clean:
	rm -f main
