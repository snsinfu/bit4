# NFS: FreeBSD server, Debian client

Server and client share a directory via a private network. Uses ipfw firewall.

```
vagrant up --no-provision
vagrant provision n-server
vagrant provision n-client
```

The client provisioning may fail with error:

```
Error mounting /srv/foo: mount.nfs: access denied by server while mounting 10.94.216.10:/data/foo
```

In this case restart the server and retry provisioning:

```
vagrant reload n-server
vagrant provision n-client
```

Log in to server and check shared directory:

```
vagrant ssh n-server
sudo ls -la /data/foo
sudo cat /data/foo/test.txt
```

Also in client:

```
vagrant ssh n-client
ls -la /srv/foo
```

From client any user can write to the shared directory. The uid:gid is mapped
to nobody:nobody on the server. I don't like this (I want to restrict access
to the shared directory by a specific user or root on the client) but anyway...

```
echo Hello > /srv/foo/hello.txt
```
