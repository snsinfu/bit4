## Installing rio v0.5.0 on a k3s cluster

Rio v0.5.0 installs its own service loadbalancer. If the standard port is used:
`rio --http-port 80 --https-port 443` the server loadbalancer conflicts with
traefik service installed on k3s cluster by default. So, to export rio apps on
the standard port, you need to disable the traefik service.

## Linkerd dashbord is exposed

First I thought it's a good idea to expose rio apps directly but it may not be
so. It also exposes the linkerd dashbord which should not be accessible publicly.

Maybe I should stick to the non-standard ports (9080 and 9443) and allow access
to these ports only via VPN. And somehow reverse-proxy whitelisted external
access to rio's gateway (but how?).

```
+------------+   myapp.example.com     +---------------+
|  internet  | ----------------------> |  k3s traefik  |
+------------+                         +---------------+
                                               |
                                               | (how?)
                                               V
 +-------+   myapp.xxxxxx.on-rio.io    +---------------+
 |  vpn  | --------------------------> |  rio gateway  |
 +-------+                             +---------------+
                                               |
                                               |
                                               V
                                          +---------+
                                          |  myapp  |
                                          +---------+
```
