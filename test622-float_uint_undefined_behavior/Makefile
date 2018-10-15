OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++14 \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS)


.PHONY: run clean
.SUFFIXES: .cc .s

all: main main.s
	./main

clean:
	rm -f main main.s

.cc.s:
	$(CXX) $(CXXFLAGS) -S -o $@ $<
