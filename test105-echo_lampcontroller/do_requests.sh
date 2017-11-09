#!/bin/sh

API_BASE="http://localhost:1323"

# success
curl -sL -X GET "${API_BASE}/lamps"
curl -sL -X PUT "${API_BASE}/lamps/1" -F "green=123" -F "blue=100"
curl -sL -X GET "${API_BASE}/lamps/1"

# error
curl -sL -X GET "${API_BASE}/lamps/10"
curl -sL -X PUT "${API_BASE}/lamps/-1"
curl -sL -X PUT "${API_BASE}/lamps/abc"
