#!/bin/sh
set -eux
exec 2>/var/log/firstboot.log

# Grow filesystem on the last partition.
gpart recover da0
gpart resize -i 2 da0
growfs -y da0p2

# Hostname
sysrc hostname="$(fetch -o- http://169.254.169.254/latest/meta-data/hostname)"

# Host key
rm -f /etc/ssh/ssh_host_*
ssh-keygen -A
if pgrep sshd; then
    service sshd restart
fi

# User script
fetch -o- http://169.254.169.254/lastest/user-data | sh
