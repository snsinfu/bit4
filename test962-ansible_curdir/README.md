# Ansible file organization

```
/
+-- ansible/    playbooks, roles and plugins
+-- assets/     src files for copy and template modules
```

The src path is relative to playbook, so the path should be prefixed by
`../assets/`. Namely,

```yaml
- name: service configuration is up
  copy:
    src: ../assets/some-service.conf
    dest: /etc/some-service.conf
```

Use subdirectory under assets directory for host-dependent configuration:

```yaml
- name: service configuration is up
  copy:
    src: ../assets/{{ inventory_hostname }}/some-service.conf
    dest: /etc/some-service.conf
```
