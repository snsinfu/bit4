ARTIFACTS = \
  main      \
  trajectory.sqlite3

INCLUDES =                    \
  -isystem /usr/local/include \
  -isystem ../include

OPTFLAGS = \
  -O2

LDFLAGS =   \
  -lsqlite3 \
  -lz

CXXFLAGS =            \
  -std=c++2a          \
  -pedantic           \
  -Wall               \
  -Wextra             \
  -Wconversion        \
  -Wsign-conversion   \
  -Wshadow            \
  -Wno-c99-extensions \
  $(INCLUDES)         \
  $(OPTFLAGS)         \
  $(EXTRA_CXXFLAGS)


.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)

main: main.cc *.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ main.cc
