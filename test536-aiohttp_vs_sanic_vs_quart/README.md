Platform: macOS 10.13.6, Core i5 6267U 2.9GHz 2C4T

# Client side timing

Results of three measurements (unit: seconds):

```
yes http://127.0.0.1:8080/resource/1111 | head -n 1000 | time xargs -L1 -P10 curl -fsS -o /dev/null
aiohttp     4.090   4.526   4.344
flask       4.191   3.905   4.260
sanic       4.048   4.089   4.265
quart       4.822   4.941   4.880
echo(go)    3.692   3.577   4.023
```

```
yes http://127.0.0.1:8080/resource/1111 | head -n 1000 | time xargs -L1 -P20 curl -fsS -o /dev/null
aiohttp     4.341   4.436   4.424
flask       4.352   4.349   4.236
sanic       4.031   3.986   4.152
quart       4.800   4.857   4.422
echo(go)    3.865   4.033   3.983
```

```
yes http://127.0.0.1:8080/resource/1111 | head -n 1000 | time xargs -L1 -P100 curl -fsS -o /dev/null
aiohttp     4.510   4.169   4.496
flask       3.894   3.976   4.514
sanic       3.828   4.106   4.029
quart       4.771   4.821   4.622
echo(go)    3.552   3.922   3.844
```

The four python frameworks performed almost the same. Sanic is a bit faster but
the differences are not that big. These tests might just have been measuring the
performance of curl.

# Server side load

```
time pipenv run python x_app.py >/dev/null 2>&1

-n 1000 | -P 100
aiohttp     1.44 user   0.16 system
flask       1.70 user   0.24 system
sanic       0.79 user   0.13 system
quart       2.06 user   0.17 system
echo(go)    0.84 user   0.35 system
```

Sanic is significantly less loaded.

# Profiles

```
pipenv run python -m cProfile x_app.py
```

cumtime:

```
aiohttp     web_protocol.py:342(start)      3.414
            web_response.py:578(write_eof)  1.646
            web_response.py:391(write_eof)  1.499
            http_writer.py:102(write_eof)   1.197
            http_writer.py:51(_write)       1.099
            ...

flask       ?

sanic       app.py:519(handle_request)      0.514
            server.py:327(write_response)   0.435
            server.py:304(log_response)     0.354
            __init__.py:1322(info)          0.287
            __init__.py::1445(_log)         0.224
            ...

quart       asgi.py:51(_handle_request)     3.409
            asgi.py:58(_send_response)      2.388
            h11.py:224(asgi_send)           2.146
            h11.py:205(send)                1.167
            app.py:1416(handle_request)     0.997
            ...

echo        net/http.(*conn).serve          0.160
            echo.(*Echo).ServeHTTP          0.090
            ...
```

aiohttp and Quart performed similarly and aiohttp was a bit faster. Sanic looks
insanely fast... I doubt that the measurements were really correct. Maybe I'm
missing something on profiling async code. Or Sanic was actually that fast.
