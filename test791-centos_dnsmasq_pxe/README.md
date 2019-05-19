## Served files

```
/srv
│
├── tftp
│   │
│   ├── images
│   │   │
│   │   └── centos7
│   │       ├── vmlinuz
│   │       └── initrd.img
│   │
│   ├── pxelinux.cfg
│   │   └── default
│   │
│   ├── pxelinux.0
│   ├── menu.c32
│   ├── memdisk
│   ├── mboot.c32
│   └── chain.c32
│
├── www
│   │
│   └── inst
│       │
│       └── centos7
│           ├── squashfs.img
│           └── ks.cfg
│
└── CentOS-7-x86_64-NetInstall-1810.iso
```

`tftp` directory is served by dnsmasq and `www` directory is served by nginx.

