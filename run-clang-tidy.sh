#!/bin/sh -efu
set -efu

# https://clang.llvm.org/docs/DiagnosticsReference.html
# Warnings are reported as clang-diagnostic-*.
FLAGS=""
FLAGS="${FLAGS} -isystem include"
FLAGS="${FLAGS} -Wpedantic -Wall -Wextra -Wmost -Wshadow -Wconversion"
FLAGS="${FLAGS} -Weffc++"
FLAGS="${FLAGS} -std=c++14"

# http://clang.llvm.org/extra/clang-tidy/
clang-tidy -checks='*' "$@" -- ${FLAGS} ${CXXFLAGS:-}
