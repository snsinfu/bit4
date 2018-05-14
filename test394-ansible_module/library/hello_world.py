#!/usr/bin/python

import json

from ansible.module_utils.basic import AnsibleModule


DEFAULT_MESSAGE = 'Hello, world!'


def main():
    mod = AnsibleModule(argument_spec={'message': {'required': False,
                                                   'default': DEFAULT_MESSAGE}})
    hello_world(mod)


def hello_world(mod):
    mod.exit_json(msg=mod.params['message'], changed=False)


if __name__ == '__main__':
    main()
