# Golang Let's Encrypt TLS server

Set these environment variables:

```
HCLOUD_TOKEN=...
CLOUDFLARE_EMAIL=...
CLOUDFLARE_TOKEN=...
```

Create .vars.json containing at least these fields:

```
{
  "domain": "your cloudflare domain name",
  "primary_pubkey": "ssh pubkey",
  "admin_user": "ssh user",
  "email": "Email address used for acquiring Let's Encrypt certs"
}
```

Then, run these commands:

```
$ make
$ make start
$ make test
```
