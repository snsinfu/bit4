INCLUDES = \
  -isystem /usr/local/include

DBGFLAGS = \
  -g       \
  -fsanitize=address

OPTFLAGS = \
  -flto    \
  -O1

CXXFLAGS =            \
  -std=c++14          \
  -pedantic           \
  -Wall               \
  -Wextra             \
  -Wconversion        \
  -Wshadow            \
  -Wno-missing-braces \
  $(OPTFLAGS)         \
  $(DBGFLAGS)         \
  $(INCLUDES)

OBJECTS =     \
  lda.o       \
  lda_test.o  \
  ijk_test.o  \
  math_test.o \
  test.o

.SUFFIXES: .cc .hpp
.PHONY: run clean

run: test
	ASAN_OPTIONS=detect_odr_violation=0 ./test

clean:
	rm -f test $(OBJECTS)

test: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lda.o: lda.hpp ijk.hpp math.hpp
ijk_test.o: ijk.hpp
lda_test.o: lda.hpp
math_test.o: math.hpp
