all:
  hosts:
    dokku:
      ansible_user: hetzner
      ansible_host: ${server_address}
      dokku_device: ${dokku_device}
