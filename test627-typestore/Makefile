CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow


.PHONY: all clean
.SUFFIXES: .cc

run: main
	@./main

clean:
	rm -f main

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
