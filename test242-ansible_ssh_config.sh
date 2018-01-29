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
awk '{
    print "Host " $2
    print "  HostName " $1
    print "  User " $4
    print "  Port " $3
    print "  IdentityFile " $5
    print "  IdentitiesOnly yes"
}'
