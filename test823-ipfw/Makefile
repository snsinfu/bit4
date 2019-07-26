# https://github.com/squat/terraform-provider-vultr/issues/84
TERRAFORM = terraform-0.11

CONFIG_FILES = \
  templates/ipfw.rules.j2

ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _vars.json \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _ssh_config \
  _hosts \
  _id_ed25519 \
  _id_ed25519.pub

.PHONY: all clean destroy ssh mosh

all: _provision.ok
	@:

clean: destroy
	rm -rf $(ARTIFACTS)

destroy:
	$(TERRAFORM) destroy -auto-approve server
	@rm -f _server.ok

ssh: _ssh_config
	ssh -F _ssh_config vpnserver

mosh: _ssh_config
	mosh --ssh='ssh -F _ssh_config' vpnserver

_init.ok:
	$(TERRAFORM) init server
	@touch $@

_server.ok: _init.ok _vars.json _id_ed25519 server/* server/templates/*
	$(TERRAFORM) apply -auto-approve -var-file _vars.json server
	@touch $@

_provision.ok: _ssh_config _hosts provision.yml $(CONFIG_FILES)
	ansible-playbook provision.yml
	@touch $@

_id_ed25519:
	ssh-keygen -t ed25519 -C tempkety -N '' -f $@

_ssh_config: _server.ok
	$(TERRAFORM) output ssh_config > $@

_hosts: _server.ok
	$(TERRAFORM) output ansible_inventory > $@

_vars.json: host_vars/*
	ansible-inventory -i hosts --host vpnserver | jq '{admin_user}' > $@
