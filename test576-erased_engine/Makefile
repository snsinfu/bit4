OPTFLAGS = \
  -O2

DBGFLAGS = \
  -g       \
  -fsanitize=address

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(DBGFLAGS)       \
  $(OPTFLAGS)

PRODUCT = run_test
OBJECTS = run_test_main.o random_engine_test.o

.SUFFIXES: .cc
.PHONY: test clean

test: $(PRODUCT)
	@./$(PRODUCT)

clean:
	rm -f $(PRODUCT) $(OBJECTS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PRODUCT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

random_engine_test.o: random_engine.hpp
