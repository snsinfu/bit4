#!/bin/sh
set -eu

# CA
openssl genrsa -out ca.key 4096
openssl req -new -key ca.key -config ca.conf -out ca.csr
openssl x509 -sha256 -days 365 -req -in ca.csr -signkey ca.key -out ca.cert

# Server
openssl genrsa -out server.key 4096
openssl req -new -key server.key -config server.conf -out server.csr
openssl x509 -sha256 -days 365 -req -in server.csr -extfile server.ext -CA ca.cert -CAkey ca.key -set_serial 1 -out server.cert

# Verify
openssl verify -CAfile ca.cert server.cert

# No longer needed
rm ca.key ca.csr server.csr
