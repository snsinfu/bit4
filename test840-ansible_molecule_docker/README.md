# Test Ansible custom virtual module using Molecule and Docker

Create a virtualenv. The pip command installs ansible, molecule, ansible-lint
and docker-py packages.

```sh
python3 -m venv .venv
source activate .venv/bin/activate
pip install -r requirements.txt
```

Run test:

```sh
molecule test
```

You may want to use `molecule converge` to reuse the same docker container for
successive test runs for development:

```sh
molecule converge
molecule converge
...
molecule destroy
...
```

Cleanup:

```sh
deactivate
rm -r .venv
```
