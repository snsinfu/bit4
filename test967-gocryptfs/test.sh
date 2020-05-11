#!/bin/sh -eu

mkdir data.cipher
mkdir data

gocryptfs -init data.cipher
gocryptfs data.cipher data

echo "The quick brown fox jumps over the lazy dog." > data/test.txt
cat data/test.txt

umount data

file data.cipher/*
