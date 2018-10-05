ARTIFACTS = \
  _init.ok \
  _server.ok \
  _provision.ok \
  terraform.tfstate \
  terraform.tfstate.backup \
  provision.retry \
  hosts.ini \
  known_hosts

SSH_OPTIONS = \
  -o UserKnownHostsFile=known_hosts \
  -o IdentitiesOnly=yes \
  -o PasswordAuthentication=no


.PHONY: all clean ssh

all: _provision.ok
	@:

clean:
	terraform destroy
	rm -rf $(ARTIFACTS)

ssh: _server.ok id_rsa
	ssh $(SSH_OPTIONS) -i id_rsa root@$$(terraform output ip)

id_rsa:
	ssh-keygen -t ed25519 -f $@

_init.ok: main.tf
	terraform init
	@touch $@

_server.ok: _init.ok main.tf id_rsa
	terraform apply -target hcloud_server.main
	@touch $@

hosts.ini: _server.ok main.tf hosts.ini.tpl
	terraform apply -target data.template_file.hosts
	terraform output inventory > $@

_provision.ok: _server.ok ansible.cfg hosts.ini id_rsa provision.yml
	ansible-playbook provision.yml
	@touch $@

_provision.ok: \
  config/ipv4_firewall.service \
  config/ipv4_firewall.sh.j2 \
  config/ipv6_firewall.service \
  config/ipv6_firewall.sh.j2 \
  config/sshd_config.j2
