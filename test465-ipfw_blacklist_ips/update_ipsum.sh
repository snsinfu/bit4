#!/bin/sh -eu

ipsum_url="https://raw.githubusercontent.com/stamparm/ipsum/master/ipsum.txt"
min_score=3

curl -fSL --compressed "${ipsum_url}" |
awk -v min_score="${min_score}" '!/#/ && $2 >= min_score' > config/ipsum.txt
