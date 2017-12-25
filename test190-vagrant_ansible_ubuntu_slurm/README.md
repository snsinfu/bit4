To execute:

    vagrant up

Re-provision after modifying the playbooks:

    vagrant provision

Or manually:

    ANSIBLE_HOST_KEY_CHECKING=false ansible-platbook -i vagrant_hosts site.yml
