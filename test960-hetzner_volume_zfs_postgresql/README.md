# PostgreSQL on ZFS on Hetzner Volume

[Hetzner FreeBSD image][freebsd-image] is rqeuired. Set the image ID, admin
user name and admin SSH pubkeys in `terraform.tfvars` file:

```
server_image = "1234567"
admin_name = "sinfu"
admin_public_keys = [
  "ssh-ed25519 AAAAC3Nza..."
]
```

Also set `HCLOUD_TOKEN` environment variable:

```
export HCLOUD_TOKEN=...
```

Then, run make to deploy a server and a volume:

```
make
```

Wait for ~2 minutes, ssh into the server, and run benchmark:

```
make ssh
sudo ./benchmark.sh
```

The script accepts zfs filesystem options as arguments.

```
sudo ./benchmark.sh atime=off compression=lz4
```

Exit from the server and cleanup:

```
make destroy clean
```

[freebsd-image]: https://github.com/snsinfu/hetzner-freebsd-image/


The benchmark results below are all run on a `cx11` host.


## Synchronous commit

### default

```
latency average = 6.909 ms
tps = 144.737674 (including connections establishing)
tps = 144.860861 (excluding connections establishing)
```

### atime=off

```
latency average = 6.345 ms
tps = 157.597703 (including connections establishing)
tps = 157.671413 (excluding connections establishing)
```

### atime=off compression=lz4

```
latency average = 6.707 ms
tps = 149.101277 (including connections establishing)
tps = 149.169716 (excluding connections establishing)
```

### atime=off compression=lz4 recordsize=16k

```
latency average = 6.863 ms
tps = 145.705243 (including connections establishing)
tps = 145.760758 (excluding connections establishing)
```

### atime=off compression=lz4 recordsize=8k

```
latency average = 5.226 ms
tps = 191.337241 (including connections establishing)
tps = 191.406443 (excluding connections establishing)
```

### atime=off compression=lz4 recordsize=8k primarycache=metadata

```
latency average = 8.522 ms
tps = 117.349995 (including connections establishing)
tps = 117.496579 (excluding connections establishing)
```


## Asynchronous commit

Results with `benchmark.sh -a`. The `-a` option sets `synchronous_commit = off`
in `postgresql.conf`.

### default

```
latency average = 2.290 ms
tps = 436.628200 (including connections establishing)
tps = 436.872365 (excluding connections establishing)
```

### atime=off

```
latency average = 2.271 ms
tps = 440.381534 (including connections establishing)
tps = 440.646867 (excluding connections establishing)
```

### atime=off compression=lz4

```
latency average = 1.412 ms
tps = 707.967625 (including connections establishing)
tps = 708.413265 (excluding connections establishing)
```

### atime=off compression=lz4 recordsize=16k

```
latency average = 1.104 ms
tps = 905.809143 (including connections establishing)
tps = 906.170731 (excluding connections establishing)
```

### (BEST) atime=off compression=lz4 recordsize=8k

```
latency average = 1.050 ms
tps = 952.239462 (including connections establishing)
tps = 952.848456 (excluding connections establishing)
```

### atime=off compression=lz4 recordsize=8k primarycache=metadata

```
latency average = 4.497 ms
tps = 222.361945 (including connections establishing)
tps = 222.563070 (excluding connections establishing)
```


## More cores and RAM

`cx41` (4 vCPU, 16GB RAM):

```
$ pgbench -U postgres -T 10 -c 4 -j 4
starting vacuum...end.
transaction type: <builtin: TPC-B (sort of)>
scaling factor: 100
query mode: simple
number of clients: 4
number of threads: 4
duration: 10 s
number of transactions actually processed: 45059
latency average = 0.890 ms
tps = 4493.750768 (including connections establishing)
tps = 4498.769330 (excluding connections establishing)
```


## Conclusion

- Hetzner volume is slow like magnetic drive but scales well with concurrency.
- ZFS options `atime=off recordsize=8k` are helpful for pgbench workload.
- PostgreSQL option `synchronous_commit = off` significantly improves performance.
- ZFS compression significantly improves performance when `synchronous_commit`
  is off.

Note: `synchronous_commit = off` should not be configured globally. It should
be set per-transaction basis. But for non-serious use case it's great.

Note: For serious use case I should be using a managed database service. So
for my hobby database deployment, setting `synchronous_commit = off` globally
makes sense actually.
