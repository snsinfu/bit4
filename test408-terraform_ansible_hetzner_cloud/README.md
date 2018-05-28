# Provision Hetzner Cloud server using Terraform and Ansible

## Prerequisites

- Terraform
- Ansible
- [terraform-provider-hcloud][1]

[1]: https://github.com/hetznercloud/terraform-provider-hcloud

## Run

Set `HCLOUD_TOKEN` environment variable to your Hetzner Cloud token and run
make:

```console
export HCLOUD_TOKEN=...
make
```

This will generate a ssh key, launches a cloud server and provisions the server
with ansible. You can then ssh or mosh into the server as follows:

```console
make ssh
make mosh
```

To destroy the server:

```console
make clean
```
