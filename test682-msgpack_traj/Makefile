OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++2a \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  -Wno-c99-extensions \
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
