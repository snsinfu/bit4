import os

from ansible.constants import config
from ansible.plugins import AnsiblePlugin
from ansible.plugins.vars import BaseVarsPlugin
from ansible.utils.vars import merge_hash


# TODO: Make dirname customizable via ansible.cfg.
# -> Current release (2.9) does not have a unified way of configuring vars
# plugins. That feature will be added in 2.10. Until then, we just reference
# the environment variable directly.
# See: https://github.com/ansible/ansible/commit/c1f1b2029c425665622608001207267e8f2dc176

DOCUMENTATION = """
vars: config_vars
short_description: Merge variables from files in config directory
options:
  dirname:
    type: str
    ini:
      - key: dirname
        section: config_vars
    env:
      - name: ANSIBLE_CONFIG_VARS_DIRNAME
"""

DEFAULT_DIRNAME = "config"


# BaseVarsPlugin does not inherit AnsiblePlugin in < 2.10.
class VarsModule(AnsiblePlugin, BaseVarsPlugin):
    def get_vars(self, loader, path, entities, cache=True):
        super().get_vars(loader, path, entities)

        dirname = DEFAULT_DIRNAME
        try:
            dirname = self.get_option("dirname")
        except KeyError:
            pass
        dirname = os.getenv("ANSIBLE_CONFIG_VARS_DIRNAME") or dirname

        merged_vars = {}

        config_path = os.path.realpath(os.path.join(self._basedir, dirname))
        if not os.path.isdir(config_path):
            return merged_vars

        for name in sorted(os.listdir(config_path)):
            file_path = os.path.join(config_path, name)
            loaded_vars = loader.load_from_file(file_path)
            merged_vars = merge_hash(merged_vars, loaded_vars)

        return merged_vars
