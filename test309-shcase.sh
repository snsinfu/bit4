#!/bin/sh -eu

input="$1"

case ${input}
in yes)     choice="y"
;; no)      choice="n"
;; cancel)  choice="c"
esac

echo ${choice}
