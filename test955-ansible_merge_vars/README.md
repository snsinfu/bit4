# Merge dictionaries from multiple vars files

`hash_behaviour=merge` is a global setting and not recommended. `vars_files`
attribute and `include_vars` module do not support merging. So I created an
action plugin that includes variable files and merge variables in given order.

See:

- https://stackoverflow.com/questions/35554415/in-ansible-how-to-combine-variables-from-separate-files-into-one-array
