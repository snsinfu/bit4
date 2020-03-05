import json


def tfvars(variables):
    def do_format(v):
        if isinstance(v, (list, dict)):
            return json.dumps(v)
        return v
    return {k: do_format(v) for k, v in variables.items()}


class FilterModule:
    def filters(self):
        return {"tfvars": tfvars}
