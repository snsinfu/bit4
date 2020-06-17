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

## uid/gid wire format incompatibility?

I searched FreeBSD src for "stringtouid" and found this:

https://github.com/freebsd/freebsd/blob/release/12.1.0/sys/fs/nfs/nfs_commonsubs.c#L3187

```c
	/* If a string of digits and an AUTH_SYS mount, just convert it. */
	str0 = str;
	tuid = (uid_t)strtoul(str0, &endstr, 10);
	if ((endstr - str0) == len) {
		/* A numeric string. */
		...
```

So, FreeBSD expects NFS user name (and group name similarly) to be completely
made of uid digits, not in the form "uid@domain". Linux client might be sending
uid/gid in the latter form.
