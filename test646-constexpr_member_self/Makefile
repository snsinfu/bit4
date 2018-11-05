CXXFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -O2

OBJECTS = a.o b.o main.o

.PHONY: all clean

run: main
	@./main

clean:
	rm -f $(OBJECTS) main

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

a.o: vec.hpp
b.o: vec.hpp
