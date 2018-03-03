The playbook may be run from both the control node and the worker node.

Control node:

    vagrant provision

Worker node:

    ansible-playbook -i local-inventory worker-node.yml

This way a playbook can be executed from a control node or a worker node using
ansible-pull. Or maybe we should just make different playbooks and include
common tasks...
