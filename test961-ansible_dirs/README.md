# Configuring Ansible directories

Sometimes I want to organize playbooks, roles and plugins in directories
different from Ansible's defaults. Playbooks especially are better organized
when placed in a distinct directory IMO.

```
.
|
+-- config
|   +-- variables.yml
|
+-- inventory
|   +-- hosts
|
+-- playbooks
|   +-- play_1.yml
|   +-- play_2.yml
|
+-- ansible
|   +-- roles
|   |   +-- report
|   |   +-- report
|   |
|   +-- vars_plugins
|       +-- config_vars.py
|
+-- ansible.cfg

```

Playbook `play_1.yml` is a flat playbook. Playbook `play_2.yml` uses a role in
`ansible/roles` directory. Test:

```console
$ ansible-playbook playbooks/play_1.yml
$ ansible-playbook playbooks/play_2.yml
```

The ansible configuration `DEFAULT_ROLES_PATH` tells Ansible where to look
for roles. This is relative to the working directory of `ansible-playbook`
command, not playbook directory. So we can organize playbooks and roles in
different hierarchy.

Reference:

- https://docs.ansible.com/ansible/latest/reference_appendices/config.html
