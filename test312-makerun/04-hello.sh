#!/bin/sh

echo "Hello, shell"

for arg in "$@"; do
    echo "Arg: ${arg}"
done
