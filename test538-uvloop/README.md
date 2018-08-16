```
yes http://127.0.0.1:8080/resource/1111 | head -n10000 | time xargs -L100 -P100 curl -fsS > /dev/null
```

## Default busiest 10 functions

```
pipenv run python -m cProfile aiohttp_app.py 2>/dev/null

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        5    0.001    0.000    6.002    1.200 base_events.py:506(run_forever)
      249    0.046    0.000    6.001    0.024 base_events.py:1662(_run_once)
    20925    0.034    0.000    4.279    0.000 events.py:86(_run)
    20925    0.032    0.000    4.245    0.000 {method 'run' of 'Context' objects}
    10200    0.258    0.000    3.719    0.000 web_protocol.py:342(start)
      249    0.011    0.000    1.652    0.007 selectors.py:553(select)
      453    1.638    0.004    1.638    0.004 {method 'control' of 'select.kqueue' objects}
    10000    0.028    0.000    1.550    0.000 web_protocol.py:298(log_access)
    10000    0.149    0.000    1.522    0.000 helpers.py:460(log)
    10000    0.062    0.000    0.840    0.000 web_app.py:313(_handle)

Client-side time
    4.474 s
    4.539 s
    4.708 s
```

## uvloop busiest 10 functions

We see about 20% improvement in `web_protocol.py`.

```
pipenv run python -m cProfile aiohttp_app.py --uvloop 2>/dev/null

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        1    1.527    1.527    4.762    4.762 {method 'run_forever' of 'uvloop.loop.Loop' objects}
    10200    0.193    0.000    2.799    0.000 web_protocol.py:342(start)
    10000    0.017    0.000    1.181    0.000 web_protocol.py:298(log_access)
    10000    0.112    0.000    1.164    0.000 helpers.py:460(log)
    10000    0.058    0.000    0.808    0.000 web_app.py:313(_handle)
    10000    0.029    0.000    0.639    0.000 __init__.py:1322(info)
    10000    0.024    0.000    0.606    0.000 __init__.py:1445(_log)
    10000    0.043    0.000    0.410    0.000 web_protocol.py:223(data_received)
    10000    0.050    0.000    0.402    0.000 aiohttp_app.py:12(resource)
    80000    0.051    0.000    0.383    0.000 helpers.py:457(<genexpr>)

Client-side time
    3.801 s
    3.778 s
    3.828 s
```
