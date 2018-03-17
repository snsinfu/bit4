CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -Wconversion -Wshadow $(X_INCLUDES)
LDFLAGS = -L /usr/local/lib
INCLUDES = -isystem /usr/local/include

ESCRIPT = /usr/local/bin/escript
GORUN = /usr/local/bin/go run
SH = /bin/sh
PYTHON = /usr/local/bin/python3

.PHONY: default
.SUFFIXES: .cc .erl .go .sh .py

default:
	@:

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
	./$@ $(ARGS)
	rm $@

.erl:
	$(ESCRIPT) $< $(ARGS)

.go:
	$(GORUN) $< $(ARGS)

.sh:
	$(SH) $< $(ARGS)

.py:
	$(PYTHON) $< $(ARGS)
