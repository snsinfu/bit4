Wireguard relaying. Relay and gateway servers are deployed on Vultr.

## Keep alive

Gateway server need to enable `PersistentKeepAlive` to make its UDP port known
by the relay server. In this example the gateway server is public so `EndPoint`
could also work, but in actual deployment gateway server would be behind a NAT
where this hole punching-like configuration works great.
