# Rio NFS persistent volume

Install k3s and rio:

```console
$ vagrant up
...

$ env KUBECONFIG=$PWD/_k3s.yaml

$ rio install
...
```

Deploy "railed-adjusts" application with NFS persistent volume:

```console
$ cd railed-adjusts

$ kubectl apply volume.yaml
persistentvolume/railed-adjusts created
persistentvolumeclaim/railed-adjusts created

$ kubectl get pvc
NAME             STATUS   VOLUME           CAPACITY   ACCESS MODES   STORAGECLASS   AGE
railed-adjusts   Bound    railed-adjusts   10Gi       RWX            nfs            5s

$ rio up

$ rio ps
```

Build succeeds but the app fails. FIXME
