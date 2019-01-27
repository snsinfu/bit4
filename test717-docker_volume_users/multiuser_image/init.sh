#!/bin/sh -eux

ls /home | awk '{ print $1, uid++ }' uid=5000 | while read user uid; do
    adduser -D -s /bin/sh -u ${uid} ${user}
    chown -R ${user}:${user} "/home/${user}"
done

exec "$@"
