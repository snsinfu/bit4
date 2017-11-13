#!/bin/sh

export DEBIAN_FRONTEND="noninteractive"

USERNAME="ubuntu"
PASSWORD="ubuntu"
USERHOME="/home/${USERNAME}"
FILESDIR="/vagrant"

main() {
    set_password
    install_dovecot
    systemctl restart dovecot
}

set_password() {
    echo "${USERNAME}:${PASSWORD}" | chpasswd
}

install_dovecot() {
    apt-get -y install dovecot-imapd

    CONFFILE="/etc/dovecot/local.conf"
    install -o root -g root -m 600 "${FILESDIR}/dovecot-local.conf" "${CONFFILE}"

    PASSFILE="$(sed -n '/scheme=cram-md5/p' "${CONFFILE}" | awk '{ print $NF }')"
    install -o root -g dovecot -m 640 /dev/null "${PASSFILE}"

    PASSHASH="$(doveadm pw -u "${USERNAME}" -p "${PASSWORD}")"
    echo "${USERNAME}:${PASSHASH}" >> "${PASSFILE}"
}

main
