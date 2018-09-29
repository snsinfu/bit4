Kuberenetes postgres + pgAdmin

```console
kubectl create -f postgres.yaml
kubectl create -f pgadmin.yaml
minikube service pgadmin
```

Login to pgAdmin with "nobody@example.com" and "password". Add a postgres server
with IP address assigned by kubernetes. The user is "postgres" and the password
is "password". The IP address can be found via the following command.

```console
kubectl get services
```
