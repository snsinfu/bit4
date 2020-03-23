from ansible.errors import AnsibleError
from ansible.plugins.action import ActionBase
from ansible.utils.vars import merge_hash


class ActionModule(ActionBase):
    def run(self, task_vars=None):
        name = self._task.action
        args = self._task.args.copy()

        try:
            filenames = args.pop("files")
        except KeyError as e:
            arg = e.args[0]
            raise AnsibleError(f"{arg} is required for {name}")

        for arg in args:
            raise AnsibleError(f"{arg} is not a valid argument for {name}")

        result = super().run(task_vars=task_vars)
        try:
            result["ansible_facts"] = self._merge_vars_files(filenames)
        except AnsibleError as e:
            result["failed"] = True
            result["message"] = str(e)
        return result

    def _merge_vars_files(self, filenames):
        merged_vars = {}
        for filename in filenames:
            loaded_vars = self._loader.load_from_file(filename)
            merged_vars = merge_hash(merged_vars, loaded_vars)
        return merged_vars
