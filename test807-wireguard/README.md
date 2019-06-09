Create .env file (the password is the one used to encrypt host vars
in this example):

```
DIGITALOCEAN_TOKEN=...
VAULT_PASSWORD=example-pass-28ae508500759cc058b3a20f3d28fc7d
```

Source `.env` and run make:

```console
$ source .env
$ make
$ make provision
$ make destroy clean
```
