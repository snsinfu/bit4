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

However, I could not deploy an example app (helloworld) using `rio up`. `rio ps`
says:

```
NAME                      IMAGE     ENDPOINT             PORTS     SCALE     WEIGHT    CREATED          DETAIL
helloworld-web@5820dd8c             No ports specified             1                   19 minutes ago   helloworld-web: not ready; ServiceDeployed: failed to create default/helloworld-web apps/v1, Kind=Deployment for service default/helloworld-web: Deployment.apps "helloworld-web" is invalid: spec.template.spec.containers: Required value(Error); helloworld-web waiting on build
```

Is this a Rio issue? Or is somehting wrong in my Riofile?
