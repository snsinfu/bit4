```console
socat udp-listen:5555 stdout
socat udp-listen:5556 stdout
udpsend 127.0.0.1:5555 127.0.0.1:5556
```
