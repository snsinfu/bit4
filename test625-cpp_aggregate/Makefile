CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wc99-extensions


.PHONY: all clean
.SUFFIXES: .cc

run: main
	@./main

clean:
	rm -f main

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
