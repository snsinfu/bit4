# Warp on Scaleway

Trying out [Cloudflare Warp](https://warp.cloudflare.com/) on a Scaleway VPS.

## Requirements

- Cloudflare account
- Scaleway account
- Vagrant and Ansible
- `vagrant install vagrant-scaleway dotenv`

## Procedure

- Get a domain (your.domain) and register it to your Cloudflare account
- Get your cert for the domain from https://www.cloudflare.com/a/warp
- Create .env file and put these environment variables:

    SCW_ORGANIZATION=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    SCW_TOKEN=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    SCW_PRIVKEY=id_rsa for your Scaleway account
    WARP_HOSTNAME=hellowarp.your.domain

- `vagrant up`
- Wait...
- Open https://`WARP_HOSTNAME` in your browser

## Notes

- The hostname must not be manually registered to the Cloudflare DNS. The warp
  system automatically registers a CNAME entry there.
- I tested this procedure on 2018-01-19. Warp was still in beta and things may
  have changed.
