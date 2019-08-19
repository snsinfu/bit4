## Dokku directories

```
/home/dokku/
│
├─ ENV
├─ HOSTNAME
├─ VHOST
├─ .sshcommand
│
├─ .ssh/
│  └─ authorized_keys
│
└─ {app}
   ├─ DOCKER_OPTIONS_DEPLOY
   ├─ DOKKU_SCALE
   ├─ ENV
   ├─ HEAD
   ├─ URLS              --- cf. dokku domains:add
   ├─ VHOST             --- cf. dokku domains:add
   ├─ CONTAINER.web.1
   ├─ IP.web.1
   ├─ PORT.web.1
   ├─ ...
   └─ nginx.conf

/var/lib/dokku/
│
├─ config/
│  ├─ buildpacks/
│  ├─ git/
│  ├─ network/
│  ├─ postgres/
│  ├─ resource/
│  ├─ scheduler-docker-local/
│  └─ ...
│
├─ core-plugins/
│  ├─ available/
│  ├─ enabled/
│  └─ config.toml
│
├─ plugins/
│  ├─ available/
│  ├─ enabled/
│  └─ config.toml
│
├─ data/
│  ├─ nginx-vhosts/
│  ├─ ps/
│  ├─ scheduler-docker-local/
│  └─ storage/
│
├─ services/
│  ├─ postgres/
│  │  ├─ {database}/
│  │  │  ├─ data/
│  │  │  │  ├─ PG_VERSION
│  │  │  │  └─ ...
│  │  │  ├─ DATABASE_NAME
│  │  │  ├─ ENV
│  │  │  ├─ ID
│  │  │  ├─ LINKS
│  │  │  └─ PASSWORD
│  │  └─ ...
│  └─ ...
│
└─ ...
```

## Operations

Create a VM:

```console
$ vagrant up
```

Install postgres:

```console
$ vagrant ssh
vagrant $ sudo dokku plugin:install https://github.com/dokku/dokku-postgres.git postgres
vagrant $ exit
```

Create an app:

```console
$ cp -r crud ~/crud
$ cd ~/crud
$ git init
$ DOKKU_HOST=dokku.me dokku apps:create crud
$ dokku postgres:create crud
$ dokku postgres:link crud crud
$ git push dokku master
```

Check:

```console
$ dokku postgres:connect crud
> \dt
> select * from users;
```
