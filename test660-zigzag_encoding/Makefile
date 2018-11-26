TARGETS = main main.s

OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

.PHONY: all clean
.SUFFIXES: .s

all: $(TARGETS)
	@:

clean:
	rm -f $(TARGETS)

.cc.s:
	$(CXX) $(CXXFLAGS) -S -masm=intel -o $@ $<
