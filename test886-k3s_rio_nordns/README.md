Accessing to Linkerd dashboard without rdns. First create a K3s cluster:

```
export HCLOUD_TOKEN=...
make
```

Then deploy Rio with rdns disabled:

```
export KUBECONFIG=$PWD/_k3s.yaml
rio install --http-port 80 --https-port 443 --disable-features rdns
cd myapp
rio up
```

Find out the linkerd-web endpoint:

```
kubectl get services -n rio-system
```

Suppose the IP address is 10.43.233.218 and the port is 8084. Then, reverse
tunnel to the endpoint via SSH:

```
ssh -F _ssh_config -L localhost:8084:10.43.233.218:8084 master
```

Open http://localhost:8084 in your browser. You should see Linkerd dashboard.
