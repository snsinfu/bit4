OPTFLAGS = \
  -O2

INCLUDES = \
  -isystem /usr/local/include

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS) \
  $(INCLUDES) \
  $(EXTRA_CXXFLAGS)

ARTIFACTS = \
  main

.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)
