#!/bin/sh -eu

: ${PORT:=3000}

for i in 1 2 3 4 5; do
    curl -fsS "http://localhost:${PORT}/" &
done

wait
