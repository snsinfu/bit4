```
cd base_server
make
```

Go to Hetzner cloud dashboard and open the console. Start FreeBSD installation.
Partition the disk like this (swap is optional):

```
p1 boot
p2 swap
p3 /
```

No root password. Do not add user. No service or hardening. Drop to the shell
and run imprinting script:

```
fetch -o- http://169.254.169.254/latest/user-data | sh
```

Exit the shell and shutdown the server. Take snapshot and attach a label
`custom_image=freebsd-12.1` to the snapshot.
