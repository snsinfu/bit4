# NFSv4 uid:gid pass-through / Linux server and Linux client

```
tshark -r server.pcap -Vx
```

I searched tshark packet dissection for the uid (4613) and gid (4512) and the
IDs appeared in a server response (GETATTR) after file creation (OPEN).

1. Client sends OPEN (OPEN4_CREATE) and GETATTR, requesting owner and owner_group
2. Server responds to the RPCs, with owner and owner_group set to uid and gid

I first thought that the owner argument to the NFSv4 OPEN operation sets the
file owner but it's not. In captured packet the owner argument does not contain
anything looks like uid or gid. Seemingly it's related to NFSv4 delegation or
something and has nothing to do with permission.

The client does not explicitly send uid nor gid in its NFSv4 message. The
server seems to figure out uid and gid from RPC header, which always contains
ones in numeric form. So it's the server, not the client, who determines
uid:gid of newly created file when AUTH_SYS is used and idmap is disabled.

## Source

- [nfsd_create_locked](https://github.com/torvalds/linux/blob/v5.7/fs/nfsd/vfs.c#L1199)
- [nfsd_create_setattr](https://github.com/torvalds/linux/blob/v5.7/fs/nfsd/vfs.c#L1162)
- [nfsd_setattr](https://github.com/torvalds/linux/blob/v5.7/fs/nfsd/vfs.c#L366)

Linux NFS server implementation is hard to read (compared to FreeBSD's one).
I give up at this time. Anyways Linux NFS server creates new file with client
user's uid:gid when sec=sys.
