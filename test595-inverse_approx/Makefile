CXXFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -funsafe-math-optimizations \
  -O2

.SUFFIXES: .cc .s
.PHONY: all clean

all: main main.s
	@:

clean:
	rm -f main main.s

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

.cc.s:
	$(CXX) $(CXXFLAGS) -S -masm=intel -o $@ $<
