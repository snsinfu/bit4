Perf results with full core usage (full module usage for Bulldozer system):

| CPU                 | Arch      | Time    | Frequency  | IPC  |
| ------------------- | --------- | ------- | ---------- | ---- |
| Xeon E3-1226 v3     | Haswell   |  68.0 s | 3.492 GHz  | 1.07 |
| Xeon E5-2687W v3    | Haswell   |  44.6 s | 3.959 GHz  | 1.43 |
| Xeon E5-2687W v4    | Broadwell |  61.0 s | 3.156 GHz  | 1.31 |
| Opteron 6276        | Bulldozer | 131.1 s | 2.539 GHz  | 0.72 |
| Threadripper 2950X  | Zen+      |  37.4 s | 3.984 GHz  | 1.70 |
| Threadripper 2990WX | Zen+      |  48.8 s | 3.419 GHz  | 1.52 |

Threadripper 2950X is really great! It offers very high IPC and stable,
high boost frequency. 2990WX is a bit slower than 2950X possibly due to
high heat dissipation (thus lower boost frequency) and memory access
inefficiency.

## Xeon E3-1226 v3 / CentOS 7.4

```
      67750.095243      task-clock (msec)         #    0.996 CPUs utilized
             1,066      context-switches          #    0.016 K/sec
                 9      cpu-migrations            #    0.000 K/sec
               645      page-faults               #    0.010 K/sec
   236,595,739,857      cycles                    #    3.492 GHz
   252,859,966,304      instructions              #    1.07  insn per cycle
    10,144,909,186      branches                  #  149.740 M/sec
         1,256,001      branch-misses             #    0.01% of all branches
    85,930,283,378      L1-dcache-loads           # 1268.342 M/sec
     5,052,683,320      L1-dcache-load-misses     #    5.88% of all L1-dcache hits
       120,988,237      LLC-loads                 #    1.786 M/sec
            94,268      LLC-load-misses           #    0.08% of all LL-cache hits

      68.000683115 seconds time elapsed
```

## Xeon E5-2687W v3 / CentOS 7.4

```
      44606.493574      task-clock (msec)         #    0.999 CPUs utilized
             6,208      context-switches          #    0.139 K/sec
                31      cpu-migrations            #    0.001 K/sec
               646      page-faults               #    0.014 K/sec
   176,597,157,194      cycles                    #    3.959 GHz                      (50.02%)
        87,191,198      stalled-cycles-frontend   #    0.05% frontend cycles idle     (50.03%)
    10,140,240,066      stalled-cycles-backend    #    5.74% backend cycles idle      (50.04%)
   253,169,015,690      instructions              #    1.43  insn per cycle
                                                  #    0.04  stalled cycles per insn  (50.04%)
    10,153,357,277      branches                  #  227.621 M/sec                    (50.02%)
         1,996,562      branch-misses             #    0.02% of all branches          (49.98%)
   116,305,678,200      L1-dcache-loads           # 2607.371 M/sec                    (49.97%)
        27,121,011      L1-dcache-load-misses     #    0.02% of all L1-dcache hits    (49.97%)
                 0      LLC-loads                 #    0.000 K/sec                    (49.97%)
                 0      LLC-load-misses           #    0.00% of all LL-cache hits     (49.99%)

      44.649631203 seconds time elapsed
```

## Xeon E5-2687W v4 / CentOS 7.4

```
      61004.138452      task-clock:u (msec)       #    1.000 CPUs utilized
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
             2,090      page-faults:u             #    0.034 K/sec
   192,525,859,702      cycles:u                  #    3.156 GHz                      (50.00%)
   252,527,963,338      instructions:u            #    1.31  insn per cycle           (62.50%)
    10,106,637,930      branches:u                #  165.671 M/sec                    (62.50%)
         1,112,376      branch-misses:u           #    0.01% of all branches          (62.50%)
    85,894,334,891      L1-dcache-loads:u         # 1408.008 M/sec                    (62.50%)
     5,043,645,318      L1-dcache-load-misses:u   #    5.87% of all L1-dcache hits    (62.50%)
        20,434,809      LLC-loads:u               #    0.335 M/sec                    (50.00%)
            10,767      LLC-load-misses:u         #    0.05% of all LL-cache hits     (50.00%)

      61.005057906 seconds time elapsed
```

## Opteron 6276 / CentOS 7.4

```
     130463.771882      task-clock:u (msec)       #    0.995 CPUs utilized
                 0      context-switches:u        #    0.000 K/sec
                 0      cpu-migrations:u          #    0.000 K/sec
             2,564      page-faults:u             #    0.020 K/sec
   331,284,076,845      cycles:u                  #    2.539 GHz                      (19.98%)
       636,055,214      stalled-cycles-frontend:u #    0.19% frontend cycles idle     (20.01%)
   166,470,939,976      stalled-cycles-backend:u  #   50.25% backend cycles idle      (20.04%)
   237,183,127,079      instructions:u            #    0.72  insn per cycle
                                                  #    0.70  stalled cycles per insn  (30.07%)
    10,088,352,043      branches:u                #   77.327 M/sec                    (40.04%)
         1,219,449      branch-misses:u           #    0.01% of all branches          (50.02%)
   116,011,777,948      L1-dcache-loads:u         #  889.226 M/sec                    (50.01%)
     2,891,929,318      L1-dcache-load-misses:u   #    2.49% of all L1-dcache hits    (49.99%)
     5,282,301,403      LLC-loads:u               #   40.489 M/sec                    (49.95%)
           738,985      LLC-load-misses:u         #    0.01% of all LL-cache hits     (49.97%)

     131.105424770 seconds time elapsed
```

## Threadripper 2950X / CentOS 7.4

```
      37344.693081      task-clock (msec)         #    0.999 CPUs utilized
             3,831      context-switches          #    0.103 K/sec
                 1      cpu-migrations            #    0.000 K/sec
               646      page-faults               #    0.017 K/sec
   148,769,719,309      cycles                    #    3.984 GHz                      (49.97%)
        79,340,209      stalled-cycles-frontend   #    0.05% frontend cycles idle     (49.97%)
    10,122,393,871      stalled-cycles-backend    #    6.80% backend cycles idle      (50.04%)
   252,816,924,112      instructions              #    1.70  insn per cycle
                                                  #    0.04  stalled cycles per insn  (50.04%)
    10,136,754,389      branches                  #  271.438 M/sec                    (50.04%)
         1,490,384      branch-misses             #    0.01% of all branches          (50.04%)
   116,440,948,411      L1-dcache-loads           # 3118.005 M/sec                    (50.04%)
         3,925,311      L1-dcache-load-misses     #    0.00% of all L1-dcache hits    (49.97%)
                 0      LLC-loads                 #    0.000 K/sec                    (49.97%)
                 0      LLC-load-misses           #    0.00% of all LL-cache hits     (49.96%)

      37.364673191 seconds time elapsed
```

## Threadripper 2990WX / CentOS 7.4

```
      48674.066695      task-clock (msec)         #    0.998 CPUs utilized
             6,452      context-switches          #    0.133 K/sec
                11      cpu-migrations            #    0.000 K/sec
             3,997      page-faults               #    0.082 K/sec
   166,436,734,222      cycles                    #    3.419 GHz                      (50.04%)
       108,704,873      stalled-cycles-frontend   #    0.07% frontend cycles idle     (49.95%)
    10,123,441,834      stalled-cycles-backend    #    6.08% backend cycles idle      (49.93%)
   252,612,136,099      instructions              #    1.52  insn per cycle
                                                  #    0.04  stalled cycles per insn  (49.93%)
    10,146,670,307      branches                  #  208.462 M/sec                    (49.93%)
         1,898,714      branch-misses             #    0.02% of all branches          (49.96%)
   116,466,444,196      L1-dcache-loads           # 2392.782 M/sec                    (50.05%)
         2,243,819      L1-dcache-load-misses     #    0.00% of all L1-dcache hits    (50.07%)
                 0      LLC-loads                 #    0.000 K/sec                    (50.07%)
                 0      LLC-load-misses           #    0.00% of all LL-cache hits     (50.07%)

      48.756456960 seconds time elapsed
```
