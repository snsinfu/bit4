#!/bin/sh -eu

curl -f \
     -X DELETE \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer $HCLOUD_TOKEN" \
     "https://api.hetzner.cloud/v1/images/$1"
