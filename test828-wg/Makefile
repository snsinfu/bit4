TERRAFORM = terraform-0.11

ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _id_ed25519 \
  _id_ed25519.pub \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _ssh_config \
  _hosts

.PHONY: all clean destroy

all: _provision.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	$(TERRAFORM) destroy -auto-approve servers
	@rm -f _server.ok

_init.ok:
	$(TERRAFORM) init servers
	@touch $@

_server.ok: _init.ok _id_ed25519
	$(TERRAFORM) apply -auto-approve -var-file _vars.json servers
	@touch $@

_provision.ok: _ssh_config _hosts provision.yml
	ansible-playbook provision.yml
	@touch $@

_id_ed25519:
	ssh-keygen -t ed25519 -C tempkey -N "" -f $@

_ssh_config: _server.ok
	$(TERRAFORM) output ssh_config > $@

_hosts: _server.ok
	$(TERRAFORM) output ansible_inventory > $@
