import os

from ansible.constants import config
from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import merge_hash


# TODO: Make dirname customizable via ansible.cfg.

DOCUMENTATION = """
vars: config_vars
short_description: Merge variables from files in config directory
options:
  dirname:
    ini:
      - key: dirname
        section: config_vars
    env:
      - name: ANSIBLE_CONFIG_VARS_DIRNAME
"""


class VarsModule(BaseVarsPlugin):
    def get_vars(self, loader, path, entities, dirname="config"):
        super().get_vars(loader, path, entities)

        merged_vars = {}

        config_path = os.path.realpath(os.path.join(self._basedir, dirname))
        if not os.path.isdir(config_path):
            return merged_vars

        for name in sorted(os.listdir(config_path)):
            file_path = os.path.join(config_path, name)
            loaded_vars = loader.load_from_file(file_path)
            merged_vars = merge_hash(merged_vars, loaded_vars)

        return merged_vars
