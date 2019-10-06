# Install rio to a remote k3s cluster

Add the following entry to `/etc/hosts`:

```
10.0.0.2 cluster.local
```

The domain name needs to be `cluster.local` because [it is hard-coded in Rio](
https://github.com/rancher/rio/search?q=cluster.local&unscoped_q=cluster.local).

Then, spin up a virtual machine:

```
vagrant up
```

The virtual machine is provisioned as a k3s cluster. Obtain the kubeconfig, and
remote-install Rio to the cluster:

```
vagrant ssh -c 'sudo cat /etc/rancher/k3s/k3s.yaml' | sed 's/127.0.0.1/cluster.local/' > k3s.yaml
rio --kubeconfig k3s.yaml install --lite --ip-address 10.0.0.2
```

The server address in k3s.yaml can't be the IP address because 10.0.0.2 is not
listed in the certificate. TODO: Find a way to add arbitrary domain names to the
certificate without setting cluster-domain.

### Setting kuberenetes config

You may set `KUBECONFIG` environment variable instead of passing `--kubeconfig`
option to every rio command:

```
export KUBECONFIG=k3s.yaml
rio info
```

Or you may set the configuration as the default if you do not operate on any
other kubernetes cluster:

```
cp -i k3s.yaml ~/.kube/config.yaml
```
