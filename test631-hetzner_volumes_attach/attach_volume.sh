#!/bin/sh -eu

curl -fsS \
     -X POST \
     -H "Authorization: Bearer ${HCLOUD_TOKEN}" \
     -H "Content-Type: application/json" \
     -d "{\"server\": $(terraform output server)}" \
     https://api.hetzner.cloud/v1/volumes/$(terraform output volume)/actions/attach
