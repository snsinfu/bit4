OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++14 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(INCLUDES) \
  $(OPTFLAGS)

ARTIFACTS = \
  main

.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)
