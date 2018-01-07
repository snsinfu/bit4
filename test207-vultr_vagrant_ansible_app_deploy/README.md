Provisioning a Debian stretch server with a non-root deploy user on Vultr VPS.

### Prepare a base image

First, configure your .env:

    VULTR_TOKEN=WUEY................................
    SSH_USER=deploy
    SSH_PRIVKEY=id_rsa

And create a base-image server:

    vagrant up image
    vagrant halt image

The default Debian installation on Vultr allows root login with password
authentication. The base-image server is configured to disallow that. Only
`SSH_USER` can log in to the base-image server using `SSH_PRIVKEY`.

Next, go to the Vultr control panel and create a snapshot of the base-image
server. Then, add the snapshot id to the .env file as `VULTR_IMAGE`:

    VULTR_IMAGE=c06..........
    VULTR_TOKEN=WUE.................................
    SSH_USER=deploy
    SSH_PRIVKEY=id_rsa

Destroy the base-image server as you do not need it later:

    vagrant destroy image

### Deploy the app server

Once base image is created, you can manage the app server as usual:

    vagrant up
    ...
    vagrant provision
    ...
    vagrant ssh
    ...
    vagrant destroy

The example playbook, `app_setup.yml`, installs docker-ce and some other dev
packages.
