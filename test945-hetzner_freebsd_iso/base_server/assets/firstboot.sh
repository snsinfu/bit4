#!/bin/sh
set -u

# Grow root filesystem.
rootpart=$(mount -p | awk '$2 == "/" { print $1 }')
rootdisk=${rootpart%p*}
rootpartno=${rootpart##*p}
gpart recover ${rootdisk}
gpart resize -i ${rootpartno} ${rootdisk}
growfs -y ${rootpart}

# Set hostname.
sysrc hostname="$(fetch -o- http://169.254.169.254/latest/meta-data/hostname)"
service hostname restart

# Host key
rm -f /etc/ssh/ssh_host_*
ssh-keygen -A
service sshd restart

# User script
fetch -o- http://169.254.169.254/lastest/user-data | sh
