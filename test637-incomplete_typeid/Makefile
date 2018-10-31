CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -O2
OBJECTS = main.o a.o b.o
ARTIFACTS = $(OBJECTS) main

.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
