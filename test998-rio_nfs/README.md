# FreeBSD NFS, Debain K3s, Rio using NFS persistent volume with custom uid:gid

Install vagrant, kubectl and rio executable on a Linux machine. Run these
commands:

```console
$ vagrant up
$ vagrant reload nfs
$ export KUBECONFIG=$PWD/_k3s.yaml
$ rio install --no-email --ip-address 10.94.216.11
$ cd myapp
$ kubectl apply -f volume.yaml
$ rio up
```

Now an app is deployed. The app uses NFS persistent volume on a FreeBSD machine
to store its state. Obtain the endpoint URL and call it several times:

```console
$ url=$(rio ps --format json myapp | jq -r '.Data.Service.status.endpoints[0]')
$ curl ${url}
1
$ curl ${url}
2
$ curl ${url}
3
$ curl ${url}
4
```

The number increases, so it's working! To be sure, inspect the volume on the
NFS server:

```console
$ vagrant ssh nfs -c 'ls -lah /data/nfs/myapp'
total 2
drwxr-xr-x  2 2000  2000      3B Jun 21 08:04 .
drwxr-xr-x  3 root  wheel     3B Jun 21 07:52 ..
-rw-r--r--  1 2000  2000      1B Jun 21 08:05 counter.txt
```

Yes, a 1-byte file is created and its owner is set to 2000:2000 as expected.
Success!


## Reboot FreeBSD NFS server after (re-)configuration

For some reason, the NFS server needs to be rebooted to allow mounting. Without
a reboot, the client cannot mount the NFS export due to "connection refused" or
"no such file or directory" error. During `rio up` it causes the rio app being
stuck at `PodInitializing` state (both app container and init sidecar) without
printing any useful message (it was very frustrating to diagnose!). Maybe nfsd
is not properly restarted in the playbook. Something related to mountd? Or some
kernel stuffs? I have no idea.

I'll investigate this issue in another mini-project later.
