# Ansible Collections

The collections feature is under-documented so I'm not sure but it looks like
`collections` directory is the default place for vendored collections.

```
<project>
├── collections
│   └── ansible_collections
│       └── <namespace>
│           └── <name>
│               ├── galaxy.yml
│               └── plugins
│                   ├── action
│                   ├── filter
│                   └── module
├── ansible.cfg
└── main.yml
```

`collections` directory can be changed by configuring `collections_paths`:

```toml
[defaults]
collections_poths = ./some_other_directory
```

`ansible_collections` part is fixed and not customizable. `<namespace>` and
`<name>` must match the corresponding entries in galaxy.yml:

```yaml
namespace: <namespace>
name: <name>
...
```

## Docs

- https://galaxy.ansible.com/docs/contributing/creating_collections.html
- https://github.com/ansible/ansible/tree/v2.8.3/test/integration/targets/collections
- https://github.com/ansible/ansible/blob/v2.8.3/lib/ansible/utils/collection_loader.py
