ARTIFACTS =   \
  _install.ok \
  main        \
  example.msgpack

INCLUDES = \
  -isystem /usr/local/include

OPTFLAGS = \
  -O2

LDFLAGS = \
  -lz

CXXFLAGS =            \
  -std=c++17          \
  -pedantic           \
  -Wall               \
  -Wextra             \
  -Wconversion        \
  -Wsign-conversion   \
  -Wshadow            \
  $(INCLUDES)         \
  $(OPTFLAGS)         \
  $(EXTRA_CXXFLAGS)


.PHONY: run clean

run: sample.msgpack _install.ok
	@pipenv run python load.py

clean:
	rm -f $(ARTIFACTS)
	pipenv --rm

sample.msgpack: main
	@./main

_install.ok:
	pipenv install
	@touch $@
