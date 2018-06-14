Launch the server:

```console
$ go run main.go
2018/06/14 20:55:23 Listening UDP on 127.0.0.1:4000
2018/06/14 20:55:28 Got 11 bytes from peer 127.0.0.1:60179
2018/06/14 20:55:28 Got 14 bytes from peer 127.0.0.1:60179
2018/06/14 20:55:33 Got 11 bytes from peer 127.0.0.1:60418
...
```

Monitor websocket:

```console
$ wscat --connect http://localhost:4000/session
connected (press CTRL+C to quit)
< Lorem ipsum
< dolor sit amet
< Lorem ipsum
< dolor sit amet
...
```
