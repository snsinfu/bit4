OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

ARTIFACTS = \
  main \
  example.msgpack

.PHONY: run clean

run: main
	./main

clean:
	rm -f $(ARTIFACTS)
