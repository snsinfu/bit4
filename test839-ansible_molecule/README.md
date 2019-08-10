# Testing custom Ansible module for FreeBSD with Molecule

Set up a virtualenv:

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Run molecule to run tests:

```sh
molecule converge
```

Cleanup:

```sh
molecule destroy
deactivate
rm -r .venv
```

## Testing action-based virtual module

Molecule sets these environment variables for Ansible ([see source][env]):

- `ANSIBLE_ROLES_PATH`
- `ANSIBLE_LIBRARY`
- `ANSIBLE_FILTER_PLUGINS`

But it does not set `ANSIBLE_ACTION_PLUGINS`, so action plugins are not loaded
by default in Molecule tests. So you need to configure the missing environment
variable by yourself in the `molecule.yml` file:

```yaml
provisioner:
  name: ansible
  env:
    ANSIBLE_LIBRARY: ../../plugins/module
    ANSIBLE_ACTION_PLUGINS: ../../plugins/action
```

Here `ANSIBLE_LIBRARY` is also configured to tell Molecule to load modules from
the `plugins/module` directory, which is not in Molecule's default environment
configuration.

[env]: https://github.com/ansible/molecule/blob/2.20.2/molecule/provisioner/ansible.py#L198
