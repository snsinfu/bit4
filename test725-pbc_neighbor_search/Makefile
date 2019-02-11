OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem ../include

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES)

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)
