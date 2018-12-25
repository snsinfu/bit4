ARTIFACTS = main trajectory.db

INCLUDES = \
  -isystem /usr/local/include

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lsqlite3

CXXFLAGS =           \
  -std=c++17         \
  -pedantic          \
  -Wall              \
  -Wextra            \
  -Wconversion       \
  -Wsign-conversion  \
  -Wshadow           \
  $(INCLUDES)        \
  $(OPTFLAGS)        \
  $(EXTRA_CXXFLAGS)


.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)
