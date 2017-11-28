#!/bin/sh

linehash() {
    while read -r line; do
        hash="$(echo "${line}" | md5)"
        date="$(date +%FT%T%z)"
        echo "${date}	${hash}"
    done
}

yes                    |
awk '{ print rand() }' |
pv -lL 10              |
linehash               |
multilog s10000 ./target
