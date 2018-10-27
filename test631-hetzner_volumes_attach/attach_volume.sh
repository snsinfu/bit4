#!/bin/sh -eu

server_id="$1"
volume_id="$2"

curl -fsS \
     -X POST \
     -H "Authorization: Bearer ${HCLOUD_TOKEN}" \
     -H "Content-Type: application/json" \
     -d "{\"server\": ${server_id}}" \
     https://api.hetzner.cloud/v1/volumes/${volume_id}/actions/attach
