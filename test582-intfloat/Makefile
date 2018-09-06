CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  -O2

.PHONY: clean

main: main.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc

clean:
	rm -f main
