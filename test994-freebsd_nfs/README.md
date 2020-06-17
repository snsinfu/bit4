# NFSv4 numeric uid:gid on FreeBSD server

When NFSv4 server is on FreeBSD numeric gid sent by a Linux client seems to be
not recognized by the FreeBSD server. When the server is Linux the numeric gid
is handled as expected. So I believe this should be a configuration problem.

## sysctl

`man nfsv4` says this:

> The form where the numbers are in the strings can only be used for
> AUTH_SYS.  To configure your systems this way, the nfsuserd(8) daemon
> does not need to be running on the server, but the following sysctls need
> to be set to 1 on the server.
```
      vfs.nfs.enable_uidtostring
      vfs.nfsd.enable_stringtouid
```

Setting these sysctls did not solve the issue.
