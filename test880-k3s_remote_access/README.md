# Install rio to a remote k3s cluster

Add the following entry to `/etc/hosts`:

```
10.0.0.2 vagrant-k3s.local
```

Then, spin up a virtual machine:

```
vagrant up
```

The virtual machine is provisioned as a k3s cluster. Obtain the kubeconfig, and
remote-install rio to the cluster:

```
vagrant ssh -c 'sudo cat /etc/rancher/k3s/k3s.yaml' | sed 's/127.0.0.1/vagrant-k3s.local/' > k3s.yaml
rio --kubeconfig k3s.yaml install --lite --ip-address 10.0.0.2
```

... and this does not end. `kubectl --kubeconfig k3s.yaml get pods -n rio-system`
shows error status on `linkerd-install` pods.

### Setting kuberenetes config

You may set `KUBECONFIG` environment variable instead of passing `--kubeconfig`
option to every rio command:

```
export KUBECONFIG=k3s.yaml
rio ps
```

Or you may set the configuration as the default if you do not operate on any
other kubernetes cluster:

```
cp -i k3s.yaml ~/.kube/config.yaml
```
