# Benchmarking Hetzner Cloud Volume

## Usage

Set `HCLOUD_TOKEN` environment variable. Create `terraform.tfvars` file
containing at least your ssh pubkey. You may want to also customize
`server_type` and `volume_size`.

```
admin_pubkeys = [
  "ssh-ed25519 AAAA..."
]
server_type = "cx41"
volume_size = 50
```

Type `make` to deploy the server and `make ssh` to connect to the server. The
server may take ~1min to boot.

```console
$ make
$ make ssh
```

In the server download and run `iops` benchmark:

```console
main $ curl -LO https://raw.githubusercontent.com/benschweizer/iops/master/iops
main $ chmod a+x iops
main $ sudo ./iops /dev/sdb
```

Log out of the server and destroy the server.

```console
main $ exit
$ make destroy
```


## Results

### cx11 (fsn1)

Random (single-threaded):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=1, pattern=random:
 512  B blocks:  913.6 IO/s, 467.8 kB/s (  3.7 Mbit/s)
   1 kB blocks:  853.3 IO/s, 873.8 kB/s (  7.0 Mbit/s)
   2 kB blocks:  681.0 IO/s,   1.4 MB/s ( 11.2 Mbit/s)
   4 kB blocks:  339.0 IO/s,   1.4 MB/s ( 11.1 Mbit/s)
   8 kB blocks:  107.0 IO/s, 876.5 kB/s (  7.0 Mbit/s)
  16 kB blocks:  480.5 IO/s,   7.9 MB/s ( 63.0 Mbit/s)
  32 kB blocks:  479.7 IO/s,  15.7 MB/s (125.8 Mbit/s)
  65 kB blocks:  473.1 IO/s,  31.0 MB/s (248.0 Mbit/s)
 131 kB blocks:  464.6 IO/s,  60.9 MB/s (487.2 Mbit/s)
 262 kB blocks:  414.5 IO/s, 108.7 MB/s (869.3 Mbit/s)
 524 kB blocks:  355.6 IO/s, 186.4 MB/s (  1.5 Gbit/s)
   1 MB blocks:  219.1 IO/s, 229.7 MB/s (  1.8 Gbit/s)
   2 MB blocks:   95.7 IO/s, 200.8 MB/s (  1.6 Gbit/s)
   4 MB blocks:   58.9 IO/s, 247.0 MB/s (  2.0 Gbit/s)
   8 MB blocks:   33.2 IO/s, 278.9 MB/s (  2.2 Gbit/s)
  16 MB blocks:   17.9 IO/s, 300.2 MB/s (  2.4 Gbit/s)
  33 MB blocks:    9.3 IO/s, 313.0 MB/s (  2.5 Gbit/s)
  67 MB blocks:    4.7 IO/s, 316.0 MB/s (  2.5 Gbit/s)
 134 MB blocks:    2.4 IO/s, 319.7 MB/s (  2.6 Gbit/s)
 268 MB blocks:    1.2 IO/s, 320.6 MB/s (  2.6 Gbit/s)
 536 MB blocks:    0.6 IO/s, 320.8 MB/s (  2.6 Gbit/s)
```

Sequential (single-threaded):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=1, pattern=sequential:
 512  B blocks: 349291.5 IO/s, 178.8 MB/s (  1.4 Gbit/s)
   1 kB blocks: 178229.4 IO/s, 182.5 MB/s (  1.5 Gbit/s)
   2 kB blocks: 95164.1 IO/s, 194.9 MB/s (  1.6 Gbit/s)
   4 kB blocks: 50919.0 IO/s, 208.6 MB/s (  1.7 Gbit/s)
   8 kB blocks: 27451.5 IO/s, 224.9 MB/s (  1.8 Gbit/s)
  16 kB blocks: 15061.4 IO/s, 246.8 MB/s (  2.0 Gbit/s)
  32 kB blocks: 6563.0 IO/s, 215.1 MB/s (  1.7 Gbit/s)
  65 kB blocks: 3448.0 IO/s, 226.0 MB/s (  1.8 Gbit/s)
 131 kB blocks: 1785.2 IO/s, 234.0 MB/s (  1.9 Gbit/s)
 262 kB blocks: 1084.9 IO/s, 284.4 MB/s (  2.3 Gbit/s)
 524 kB blocks:  629.4 IO/s, 330.0 MB/s (  2.6 Gbit/s)
   1 MB blocks:  308.3 IO/s, 323.3 MB/s (  2.6 Gbit/s)
   2 MB blocks:  153.7 IO/s, 322.4 MB/s (  2.6 Gbit/s)
   4 MB blocks:   77.2 IO/s, 323.8 MB/s (  2.6 Gbit/s)
   8 MB blocks:   38.8 IO/s, 325.6 MB/s (  2.6 Gbit/s)
  16 MB blocks:   19.3 IO/s, 324.0 MB/s (  2.6 Gbit/s)
  33 MB blocks:    9.7 IO/s, 323.9 MB/s (  2.6 Gbit/s)
  67 MB blocks:    4.8 IO/s, 324.3 MB/s (  2.6 Gbit/s)
 134 MB blocks:    2.4 IO/s, 323.3 MB/s (  2.6 Gbit/s)
 268 MB blocks:    1.2 IO/s, 322.0 MB/s (  2.6 Gbit/s)
 536 MB blocks:    0.6 IO/s, 320.3 MB/s (  2.6 Gbit/s)
```

Random (32x multi-threaded; can't complete):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=32, pattern=random:
 512  B blocks: 7880.9 IO/s,   4.0 MB/s ( 32.3 Mbit/s)
   1 kB blocks: 6858.4 IO/s,   7.0 MB/s ( 56.2 Mbit/s)
   2 kB blocks: 5551.0 IO/s,  11.4 MB/s ( 90.9 Mbit/s)
   4 kB blocks: 4088.2 IO/s,  16.7 MB/s (134.0 Mbit/s)
   8 kB blocks: 4116.7 IO/s,  33.7 MB/s (269.8 Mbit/s)
  16 kB blocks: 4095.6 IO/s,  67.1 MB/s (536.8 Mbit/s)
  32 kB blocks: 4133.6 IO/s, 135.4 MB/s (  1.1 Gbit/s)
  65 kB blocks: 2633.5 IO/s, 172.6 MB/s (  1.4 Gbit/s)
 131 kB blocks: 1326.8 IO/s, 173.9 MB/s (  1.4 Gbit/s)
 262 kB blocks:  877.5 IO/s, 230.0 MB/s (  1.8 Gbit/s)
 524 kB blocks:  453.9 IO/s, 238.0 MB/s (  1.9 Gbit/s)
   1 MB blocks:  227.4 IO/s, 238.5 MB/s (  1.9 Gbit/s)
   2 MB blocks:  101.7 IO/s, 213.2 MB/s (  1.7 Gbit/s)
   4 MB blocks:   64.0 IO/s, 268.5 MB/s (  2.1 Gbit/s)
   8 MB blocks:   38.3 IO/s, 321.2 MB/s (  2.6 Gbit/s)
  16 MB blocks:   19.6 IO/s, 329.3 MB/s (  2.6 Gbit/s)
  33 MB blocks:   10.6 IO/s, 354.4 MB/s (  2.8 Gbit/s)
Killed
```

Sequential (32x multi-threaded; can't complete):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=32, pattern=sequential:
 512  B blocks: 1023010.6 IO/s, 523.8 MB/s (  4.2 Gbit/s)
   1 kB blocks: 854717.9 IO/s, 875.2 MB/s (  7.0 Gbit/s)
   2 kB blocks: 541450.4 IO/s,   1.1 GB/s (  8.9 Gbit/s)
   4 kB blocks: 458614.2 IO/s,   1.9 GB/s ( 15.0 Gbit/s)
   8 kB blocks: 325362.3 IO/s,   2.7 GB/s ( 21.3 Gbit/s)
  16 kB blocks: 225365.7 IO/s,   3.7 GB/s ( 29.5 Gbit/s)
  32 kB blocks: 142543.8 IO/s,   4.7 GB/s ( 37.4 Gbit/s)
  65 kB blocks: 76944.8 IO/s,   5.0 GB/s ( 40.3 Gbit/s)
 131 kB blocks: 40713.0 IO/s,   5.3 GB/s ( 42.7 Gbit/s)
 262 kB blocks: 18622.8 IO/s,   4.9 GB/s ( 39.1 Gbit/s)
 524 kB blocks: 9018.7 IO/s,   4.7 GB/s ( 37.8 Gbit/s)
   1 MB blocks: 4349.2 IO/s,   4.6 GB/s ( 36.5 Gbit/s)
   2 MB blocks: 1883.3 IO/s,   3.9 GB/s ( 31.6 Gbit/s)
   4 MB blocks:  951.1 IO/s,   4.0 GB/s ( 31.9 Gbit/s)
   8 MB blocks:  558.5 IO/s,   4.7 GB/s ( 37.5 Gbit/s)
  16 MB blocks:  208.5 IO/s,   3.5 GB/s ( 28.0 Gbit/s)
  33 MB blocks:  114.5 IO/s,   3.8 GB/s ( 30.7 Gbit/s)
Killed
```


### cx41 (fsn1)

Random (32x multi-threaded):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=32, pattern=random:
 512  B blocks: 7178.6 IO/s,   3.7 MB/s ( 29.4 Mbit/s)
   1 kB blocks: 6027.7 IO/s,   6.2 MB/s ( 49.4 Mbit/s)
   2 kB blocks: 5339.7 IO/s,  10.9 MB/s ( 87.5 Mbit/s)
   4 kB blocks: 3496.3 IO/s,  14.3 MB/s (114.6 Mbit/s)
   8 kB blocks: 3853.4 IO/s,  31.6 MB/s (252.5 Mbit/s)
  16 kB blocks: 2858.6 IO/s,  46.8 MB/s (374.7 Mbit/s)
  32 kB blocks: 2007.8 IO/s,  65.8 MB/s (526.3 Mbit/s)
  65 kB blocks: 1255.4 IO/s,  82.3 MB/s (658.2 Mbit/s)
 131 kB blocks:  862.8 IO/s, 113.1 MB/s (904.7 Mbit/s)
 262 kB blocks:  626.7 IO/s, 164.3 MB/s (  1.3 Gbit/s)
 524 kB blocks:  361.5 IO/s, 189.5 MB/s (  1.5 Gbit/s)
   1 MB blocks:  190.7 IO/s, 199.9 MB/s (  1.6 Gbit/s)
   2 MB blocks:   89.2 IO/s, 187.1 MB/s (  1.5 Gbit/s)
   4 MB blocks:   61.3 IO/s, 257.0 MB/s (  2.1 Gbit/s)
   8 MB blocks:   36.3 IO/s, 304.6 MB/s (  2.4 Gbit/s)
  16 MB blocks:   19.8 IO/s, 332.1 MB/s (  2.7 Gbit/s)
  33 MB blocks:    8.7 IO/s, 292.7 MB/s (  2.3 Gbit/s)
  67 MB blocks:    3.1 IO/s, 209.6 MB/s (  1.7 Gbit/s)
```

Sequential (32x multi-threaded):

```
/dev/sdb,  53.69 G, sectorsize=512B, #threads=32, pattern=sequential:
 512  B blocks: 284651.4 IO/s, 145.7 MB/s (  1.2 Gbit/s)
   1 kB blocks: 312146.0 IO/s, 319.6 MB/s (  2.6 Gbit/s)
   2 kB blocks: 234359.2 IO/s, 480.0 MB/s (  3.8 Gbit/s)
   4 kB blocks: 266956.7 IO/s,   1.1 GB/s (  8.7 Gbit/s)
   8 kB blocks: 216722.4 IO/s,   1.8 GB/s ( 14.2 Gbit/s)
  16 kB blocks: 225103.6 IO/s,   3.7 GB/s ( 29.5 Gbit/s)
  32 kB blocks: 167467.4 IO/s,   5.5 GB/s ( 43.9 Gbit/s)
  65 kB blocks: 70648.1 IO/s,   4.6 GB/s ( 37.0 Gbit/s)
 131 kB blocks: 31671.8 IO/s,   4.2 GB/s ( 33.2 Gbit/s)
 262 kB blocks: 13900.3 IO/s,   3.6 GB/s ( 29.2 Gbit/s)
 524 kB blocks: 15485.0 IO/s,   8.1 GB/s ( 64.9 Gbit/s)
   1 MB blocks: 9881.6 IO/s,  10.4 GB/s ( 82.9 Gbit/s)
   2 MB blocks: 5171.0 IO/s,  10.8 GB/s ( 86.8 Gbit/s)
   4 MB blocks: 2743.0 IO/s,  11.5 GB/s ( 92.0 Gbit/s)
   8 MB blocks: 1333.3 IO/s,  11.2 GB/s ( 89.5 Gbit/s)
  16 MB blocks:  717.0 IO/s,  12.0 GB/s ( 96.2 Gbit/s)
  33 MB blocks:  339.7 IO/s,  11.4 GB/s ( 91.2 Gbit/s)
  67 MB blocks:   80.8 IO/s,   5.4 GB/s ( 43.4 Gbit/s)
 134 MB blocks:   33.9 IO/s,   4.5 GB/s ( 36.4 Gbit/s)
 268 MB blocks:    3.7 IO/s, 995.2 MB/s (  8.0 Gbit/s)
```


## Comments

- Surprisingly fast.
- The throughput looks bounded by 100Gbps. Ceph storage over 100GbE?
- At 4kB-random it records ~4k IOPS. Not very fast (NVMe SSD does > 100k IOPS)
  but not bad compared to magnetic disks. It's similar to a cheap SATA SSD.
