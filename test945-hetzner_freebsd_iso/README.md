```
cd base_server
make
```

Go to Hetzner cloud dashboard and open the console. Start FreeBSD installation.
Disk must be partitioned as:

```
ada0p1 boot
ada0p2 /
```

No root password. Do not add user. Drop to the shell and run imprinting script:

```
fetch -o- http://169.254.169.254/latest/user-data | sh
```

Exit the shell and shutdown the server. Take snapshot.
