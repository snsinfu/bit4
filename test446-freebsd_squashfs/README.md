# Creating and mounting squashfs on FreeBSD

Set up a virtual FreeBSD machine and login:

```console
vagrant up
vagrant ssh
```

Then,

```console
mksquashfs / os.img -e /boot -e /dev -e /home
mkdir mnt
squashfuse os.img mnt
ls -l mnt
```

Could this be used with jail to achieve docker-like experience?
