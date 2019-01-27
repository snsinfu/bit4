Mounting home directory as a volume in a multi-user container. User data
persists between container removal/creation. All experiments are done inside
a Vagrant box.

Run `vagrant up` and login to the box with `vagrant ssh`. Then, execute
`start.sh` to launch a container. Impersonate as alice to see that a file in
the home directory is correctly owned by alice.

```console
host $ vagrant up
host $ vagrant ssh
box $ ./start.sh
# su - alice
$ ls -l
-rw-r--r--    1 alice    alice            6 Jan 27 16:09 somedata.txt
```

Host requirements: Vagrant and Ansible.

## How it works

The entrypoint of the image is set to the `init.sh` script. This script
collects user names from mounted `/home` directory and runs `adduser` and
then `chown -R` to fix uid/gid.

The mapping between user names and uids is *not* guaranteed to be preserved
between container reboot if new user is added.
