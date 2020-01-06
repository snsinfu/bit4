## Stress test performance

40 concurrent client accesses.

Without pool (port 5432):

```
number of clients: 40
number of threads: 20
number of transactions per client: 200
number of transactions actually processed: 8000/8000
latency average = 16.029 ms
tps = 2495.546374 (including connections establishing)
tps = 2534.772937 (excluding connections establishing)
```

With pool (port 6432):

```
number of clients: 40
number of threads: 20
number of transactions per client: 200
number of transactions actually processed: 8000/8000
latency average = 18.232 ms
tps = 2193.953925 (including connections establishing)
tps = 2195.146461 (excluding connections establishing)
```

Database access gets slower due to the pooling. But the pooled database now
tolerates 100 clients in return:

```
number of clients: 100
number of threads: 20
number of transactions per client: 200
number of transactions actually processed: 20000/20000
latency average = 45.756 ms
tps = 2185.525894 (including connections establishing)
tps = 2185.963538 (excluding connections establishing)
```

while the unpooled one errors:

```
connection to database "postgres" failed:
FATAL:  sorry, too many clients already
FATAL:  sorry, too many clients already
connection to database "postgres" failed:
FATAL:  sorry, too many clients already
FATAL:  sorry, too many clients already
...
```
