#!/bin/sh

sync_commit=1
while getopts a opt; do
  case ${opt} in
  a)  sync_commit=0
  esac
done
shift $((OPTIND - 1))

service postgresql stop

zfs destroy data/postgresql-12
zfs create data/postgresql-12

if [ $# -ne 0 ]; then
  zfs set "$@" data/postgresql-12
fi

chown postgres:postgres /data/postgresql-12
service postgresql initdb

if [ ${sync_commit} = 0 ]; then
  echo "synchronous_commit = off" >> /data/postgresql-12/postgresql.conf
fi

service postgresql start

pgbench -U postgres -s 100 -i
pgbench -U postgres -T 10
pgbench -U postgres -T 10
