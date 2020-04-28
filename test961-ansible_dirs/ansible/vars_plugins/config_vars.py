import functools
import os

from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import merge_hash


DOCUMENTATION = """
vars: config_vars
short_description: Merge variables from files in config directory
options:
  dirname:
    type: str
    ini:
      - key: path
        section: config_vars
    env:
      - name: ANSIBLE_CONFIG_VARS_PATH
"""

DEFAULT_PATH = "config"


@functools.lru_cache
def find_vars_files(loader, basedir, path):
    return loader.find_vars_files(basedir, path)


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities):
        super().get_vars(loader, path, entities)

        # self._basedir (set by BaseVarsPlugin) is the directory containing the
        # playbook executed. We want to use ansible-playbook command's working
        # directory instead.
        basedir = os.getcwd()

        # TODO: Change to get_option once Ansible 2.10 is released.
        config_path = os.getenv("ANSIBLE_CONFIG_VARS_PATH") or DEFAULT_PATH
        vars_files = find_vars_files(loader, basedir, config_path)

        merged_vars = {}
        for file_path in sorted(vars_files):
            loaded_vars = loader.load_from_file(file_path)
            merged_vars = merge_hash(merged_vars, loaded_vars)

        return merged_vars
