import hashlib

from ansible.plugins.action import ActionBase


ID_LENGTH = 7
DEFAULT_PATH = "/etc/rc.conf"


def compute_short_hash(subject, length):
    sha1 = hashlib.sha1()
    sha1.update(subject.encode("utf-8"))
    hexhash = sha1.hexdigest()
    return hexhash[:length]


class ActionModule(ActionBase):
    def run(self, tmp=None, task_vars=None):
        result = super().run(tmp, task_vars)

        # Translate to a blockinfile task.
        task_name = self._task.name
        task_args = self._task.args

        fragment = task_args.pop("fragment")
        subject = task_args.pop("subject", task_name)
        block_id = compute_short_hash(subject, ID_LENGTH)

        blockinfile_args = {
            "path": DEFAULT_PATH,
            "create": "yes",
            "marker": "#-{mark}",
            "marker_begin": f"{block_id}: {subject}",
            "marker_end": f"{block_id}",
            "block": fragment,
        }
        blockinfile_args.update(task_args)

        blockinfile_result = self._execute_module(
            "blockinfile", blockinfile_args, task_vars=task_vars
        )
        result.update(blockinfile_result)

        return result
