Serialize some numerical simulation trajectory as a message-pack stream. Then,
analyze the result with a python script.

```console
$ make

$ ./main

$ file trajectory.pack.gz
trajectory.pack.gz: gzip compressed data, original size 299551

$ pipenv run python analyze.py
title: A simulation trajectory
id: 42
#0: mean=0.5000 std=0.2887
#1: mean=0.5001 std=0.2887
...
```
