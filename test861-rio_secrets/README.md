Using Kubernetes Secrets to pass config to app as a JSON file.

```
vagrant up
vagrant ssh
sudo rio console
```

and press `L` key to watch log.

Change Secrets:

```
cd /vagrant/myapp
vi config.json
sudo kubectl delete secret myapp
sudo kubectl create secret generic myapp --from-file config.json
```

and see how log messages change after some delay (several tens of seconds).
