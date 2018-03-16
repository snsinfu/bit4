CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -Wconversion -Wshadow $(INCLUDES)
LDFLAGS = $(LIBS)

ESCRIPT = /usr/local/bin/escript
GO = go
PYTHON = /usr/local/bin/python3

.PHONY: default
.SUFFIXES: .cc .erl .go .py

default:
	@:

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

.erl:
	sed "1s@^#!.*@#!$(ESCRIPT)@" $< > $@
	chmod u+x $@

.go:
	$(GO) build -o $@ $<

.py:
	sed "1s@^#!.*@#!$(PYTHON)@" $< > $@
	chmod u+x $@
