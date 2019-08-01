from ansible.plugins.action import ActionBase
from ansible.utils.vars import isidentifier


class ActionModule(ActionBase):
    def run(self, tmp=None, task_vars=None):
        result = super().run(task_vars=task_vars)

        args = self._task.args
        command = args.pop("command")
        fact_name = args.pop("set_fact")

        if not isidentifier(fact_name):
            result["failed"] = True
            result["msg"] = f"The variable name {fact_name} is invalid."
            return result

        command_args = args.copy()
        command_args["_raw_params"] = command
        command_result = self._execute_module(
            "command", command_args, task_vars=task_vars
        )

        result.update(command_result)
        stdout = result.pop("stdout")
        result.pop("stdout_lines")
        result["ansible_facts"] = {fact_name: stdout}
        result["changed"] = False

        return result
