pciconf -lv

```
vgapci0@pci0:0:2:0:     class=0x030000 card=0x00000000 chip=0xbeef80ee rev=0x00 hdr=0x00
    vendor     = 'InnoTek Systemberatung GmbH'
    device     = 'VirtualBox Graphics Adapter'
    class      = display
    subclass   = VGA
```

/boot/loader.conf

```
vmm_load="YES"
pptdevs="0/2/0"
```

/etc/rc.conf

```
cloned_interfaces="bridge0 tap0"
ifconfig_bridge0="addm em0 addm tap0"
```

/etc/sysctl.conf

```
net.link.tap.up_on_open=1
```

