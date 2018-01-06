
    ssh-keygen -t rsa -b 4096 -f id_rsa
    vagrant up
    ansible-playbook -i admin_inventory --private-key id_rsa -K second.yml
