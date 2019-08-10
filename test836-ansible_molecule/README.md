# Testing custom Ansible module for FreeBSD with Molecule

Set up a virtualenv:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install ansible molecule ansible-lint python-vagrant
```

Then run molecule:

```sh
molecule converge
```

Cleanup:

```sh
molecule destroy
deactivate
rm -rf .venv
```

### Note: Use virtualenv!

Ansible and Molecule must be installed for the exact same python version.
Homebrew Ansible is installed for python3.7 and Molecule is installed for
python2.7, so brew-installed Molecule won't work. The best way is to create a
dedicated virtualenv and install Ansible and Molecule there as shown above.
See: https://github.com/ansible/molecule/issues/1851

The virtualenv name should start with a dot (i.e., hidden), or placed outside
of the directory. Otherwise Molecule tries to lint everything inside virtualenv
tree and fails with massive amount of error messages.

### Note: Timeout

Default FreeBSD does not have python installed. So we need to install python in
`prepare.yml` play. It downloads large-ish package data so timeout failure may
occur when running the play.

### TODO: Action plugin

Here `rc_conf` module is supposed to be implemented by the corresponding action
plugin but the action plugin is not loaded during `molecule converge`. Need to
investigate how molecule works...
