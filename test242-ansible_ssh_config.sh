#!/bin/sh
set -eu

inventory=""

while getopts i: opt; do
    case "${opt}" in
    i)  inventory="${OPTARG}"
        ;;
    *)  exit 1
    esac
done
shift $((${OPTIND} - 1))

target="${1:-}"

dump_inventory() {
    if test -n "${inventory}"; then
        ansible-inventory --inventory-file "${inventory}" --list "${target}"
    else
        ansible-inventory --list "${target}"
    fi
}

dump_inventory  |
jq --raw-output \
    '._meta.hostvars
    | to_entries[]
    | [ .key
      , .value.ansible_host
      , .value.ansible_port // 22
      , .value.ansible_user // "root"
      , .value.ansible_ssh_private_key_file
      ]
    | @tsv'     |
awk -v OFS='\t' \
'{
    host = $1
    addr = $2
    port = $3
    user = $4
    key = $5
    print host, user "@" addr, "-p " port " -i " key
}'
