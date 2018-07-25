#!/bin/sh

doit() {
    echo "$2 $3 $4 -> $(./count3d $@)"
}


doit 3 0 0 0 &
doit 3 0 0 1 &
doit 3 0 0 2 &
doit 3 0 1 0 &
doit 3 0 1 1 &
doit 3 0 1 2 &
doit 3 0 2 0 &
doit 3 0 2 1 &
doit 3 0 2 2 &

doit 3 1 0 0 &
doit 3 1 0 1 &
doit 3 1 0 2 &
doit 3 1 1 0 &
doit 3 1 1 1 &
doit 3 1 1 2 &
doit 3 1 2 0 &
doit 3 1 2 1 &
doit 3 1 2 2 &

doit 3 2 0 0 &
doit 3 2 0 1 &
doit 3 2 0 2 &
doit 3 2 1 0 &
doit 3 2 1 1 &
doit 3 2 1 2 &
doit 3 2 2 0 &
doit 3 2 2 1 &
doit 3 2 2 2 &

wait
