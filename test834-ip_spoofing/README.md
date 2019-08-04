Experimenting with ipfw's anti-spoof feature.

```console
(pen) $ sudo nping -S 127.0.0.1 srv
```

I could not observe spoofed packet in srv. Looks like the spoofed packet is
dropped at Vultr's gateway.
