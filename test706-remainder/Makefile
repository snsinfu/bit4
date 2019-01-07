INCLUDES = \
  -I include

OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(INCLUDES) \
  $(OPTFLAGS)

ARTIFACTS = \
  main \
  main.s


.PHONY: all clean
.SUFFIXES: .s

all: $(ARTIFACTS)
	@./main

clean:
	rm -f $(ARTIFACTS)

.cc.s:
	$(CXX) $(CXXFLAGS) -c -S -masm=intel -o $@ $<
