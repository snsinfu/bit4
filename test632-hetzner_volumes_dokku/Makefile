ARTIFACTS = \
  .terraform \
  terraform.tfstate \
  terraform.tfstate.backup \
  bootstrap.retry \
  provision.retry \
  _init.ok \
  _server.ok \
  _provision.ok \
  ssh_key \
  ssh_key.pub \
  known_hosts \
  hosts.yml

SSH_OPTIONS = \
  -o UserKnownHostsFile=known_hosts \
  -o PasswordAuthentication=no \
  -o IdentitiesOnly=yes \
  -i ssh_key \
  -l hetzner

.PHONY: all clean ssh

all: _provision.ok
	@:

clean:
	terraform destroy -auto-approve
	rm -rf $(ARTIFACTS)

ssh: _server.ok ssh_key
	ssh $(SSH_OPTIONS) $$(terraform output ip)

_init.ok: main.tf
	terraform init
	@touch $@

_server.ok: _init.ok ssh_key main.tf
	terraform apply -auto-approve
	@touch $@

_provision.ok: _server.ok ssh_key hosts.yml provision.yml
	ansible-playbook provision.yml
	@touch $@

ssh_key:
	ssh-keygen -t ed25519 -f $@ -N ""

hosts.yml: _server.ok main.tf hosts.yml.tpl
	terraform output inventory > $@
