# Install rio to a remote k3s cluster (vagrant actually)

Add the following entry to `/etc/hosts`:

```
10.0.0.2 vagrant-k3s.local
```

Then, spin up a virtual machine:

```
vagrant up
```

The virtual machine is provisioned as a k3s cluster. Obtain the kubeconfig, and
remote-install Rio to the cluster:

```
vagrant ssh -c 'sudo cat /etc/rancher/k3s/k3s.yaml' | sed 's/127.0.0.1/vagrant-k3s.local/' > k3s.yaml
rio --kubeconfig k3s.yaml install --ip-address 10.0.0.2 --mode ingress
```

Now you can deploy an app!

```
cd myapp
rio --kubeconfig ../k3s.yaml up
```

Run `rio ps -a` and open the app endpoint in your browser. Linkerd dashboard is
also interesting.

TODO: Investigate how to enable standard port access (not :8443) in the default
`svclb` mode. As of v0.5.0 the ingress mode works but it looks like being
deprecated silently (see [021c4dc](https://github.com/rancher/rio/commit/021c4dc41c4f6c2d46d004b711985a9b4661f21d)). See these issues:

- https://github.com/rancher/rio/issues/424
- https://github.com/rancher/rio/issues/531#issuecomment-536059530

Also mentioned in [the v0.5.0 release note](https://github.com/rancher/rio/releases/tag/v0.5.0). We need to use `--http-port 80 --https-port 443` but it causes stuck gateway installation. Hmm...

## Setting kuberenetes config

Instead of passing `--kubeconfig` option to every rio command, you may set
`KUBECONFIG` environment variable:

```
export KUBECONFIG=${PWD}/k3s.yaml
rio info
```

Or you may configure and switch kubectl contexts. See:

- https://kubernetes.io/docs/tasks/access-application-cluster/configure-access-multiple-clusters/

## Domain name

A local name `vagrant-k3s.local` is used here but it could just be the IP
address: `--tls-san 10.0.0.2`. If you want to access the k3s cluster with both
the name and the IP address, use the following k3s installation options:

```
export INSTALL_K3S_EXEC="server --tls-san vagrant-k3s.local --tls-san 10.0.0.2"
```

`--tls-san` is required to verify TLS certificate when connecting to k3s REST
server remotely.

## Stuck installation

Deleting traefik service `kubectl delete service traefik` seems to "fix" the installation issue but then services are inaccessible bacause of missing load balancer. However, the `hostport` mode does not help; it also causes stuck gateway installation somehow.

These commands:

```
rio install --mode svclb --http-port 80 --https-port 443
kubectl get services -A
```

show these lines:

```
kube-system   traefik                  LoadBalancer   10.43.26.77     10.0.2.15     80:31753/TCP,443:30644/TCP,8080:31989/TCP   23m
rio-system    gateway-v0               LoadBalancer   10.43.139.156   <pending>     80:32204/TCP,443:32338/TCP                  8m52s
```

Just guessing but it looks like the gateway service cannot start because a
LoadBalancer service binding to the same ports already exists.

### Disabling the gateway feature

... does not work at all.

```
rio install --mode svclb --http-port 80 --https-port 443 --disable-features gateway
```

The `svclb-gateway` pod gets stuck. Also I see a weird URL in status report:

```
$ rio ps -a
Applications
NAME      CREATED   ENDPOINT   REVISIONS   SCALE     WEIGHT    DETAIL

Routers
NAME          URL                   OPTS      ACTION    TARGET
linkerd/web   http://web-linkerd.             to        linkerd-web,port=8084

ExternalServices
NAME      CREATED   TARGET
```

Don't do this.

### Gateway?

- https://blog.jayway.com/2018/10/22/understanding-istio-ingress-gateway-in-kubernetes/
