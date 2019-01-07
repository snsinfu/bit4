OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(OPTFLAGS)

ARTIFACTS = \
  main


.PHONY: run clean

run: main
	@./main

clean:
	rm -f $(ARTIFACTS)
