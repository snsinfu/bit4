TERRAFORM = terraform-0.11
ANSIBLE_PLAYBOOK = ansible-playbook
ANSIBLE_INVENTORY = ansible-inventory

CONFIG_FILES = \
  rc-ipfw.conf \
  rc-routing.conf \
  rc-wireguard.conf \
  ipfw.rules.j2 \
  server-wg0.conf.j2 \
  client-wg0.conf.j2

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
  _client-wg0.conf

.PHONY: all clean destroy ssh

all: _provision.ok
	@:

clean: destroy
	rm -rf $(ARTIFACTS)

destroy:
	$(TERRAFORM) destroy -auto-approve server
	@rm -f _server.ok

ssh: _ssh_config
	ssh -F _ssh_config vpnserver

_init.ok:
	$(TERRAFORM) init server
	@touch $@

_server.ok: _init.ok _vars.json server/* server/templates/*
	$(TERRAFORM) apply -auto-approve -var-file _vars.json server
	@touch $@

_provision.ok: _ssh_config _hosts provision.yml $(CONFIG_FILES)
	$(ANSIBLE_PLAYBOOK) provision.yml
	@touch $@

_ssh_config: _server.ok
	$(TERRAFORM) output ssh_config > $@

_hosts: _server.ok
	$(TERRAFORM) output ansible_inventory > $@

_vars.json: host_vars/*
	$(ANSIBLE_INVENTORY) -i tmp_hosts --host vpnserver > $@
