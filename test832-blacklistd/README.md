Looks like FreeBSD's blacklist patch for sshd does not block attack attempts
when sshd only allows pubkey authentication. That is... rather useless. There
is [a relevant forum discussion] that suggests sshguard instead.

[1]: https://forums.freebsd.org/threads/how-to-setup-sshd-blacklistd-ipfw.63126/
