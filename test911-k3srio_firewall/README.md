Install rio v0.6.0:

```
vagrant up
export KUBECONFIG=$PWD/_k3s.yaml
rio install --ip-address 192.168.119.2
rio dashboard
```

Deploy an app:

```
cd helloworld
rio up
```

I could install Rio and access to the dashboard. Looks like the iptables rules
are working.

See how Rio handles `rio up` with no argument: https://github.com/rancher/rio/blob/v0.6.0/cli/pkg/up/build.go#L27

The default riofile contains `image: ./` specification. This is required to
deploy a local stack. The path needs to be `./` exactly. `.` does not work.
