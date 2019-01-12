CXXFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -O2

TARGETS = main main.s


.PHONY: run clean
.SUFFIXES: .s

run: $(TARGETS)
	@./main

clean:
	rm -f $(TARGETS)

.cc.s:
	$(CXX) $(CXXFLAGS) -masm=intel -S -o $@ $<
