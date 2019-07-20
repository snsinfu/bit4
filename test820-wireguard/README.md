Vultr FreeBSD deployment of Wireguard. Linux has better (kernel!) support so
using FreeBSD for Wireguard is kinda dumb. But it makes sense for me because I
am planning to set up Wireguard on an existing FreeBSD server.

Requirements:

- Terraform 0.11 (not >= 0.12 due to [a plugin issue][1])
- [Vultr Terraform Provider][2]
- Env var `VULTR_API_KEY` for Vultr API access
- Valid `admin_pubkeys` in `host_vars/vpnserver.yml`

[1]: https://github.com/squat/terraform-provider-vultr/issues/84
[2]: https://github.com/squat/terraform-provider-vultr
