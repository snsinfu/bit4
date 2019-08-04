ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _id_ed25519 \
  _id_ed25519.pub \
  _known_hosts \
  _ssh_config \
  _hosts \
  _init.ok \
  _server.ok \
  _provision.ok


.PHONY: all clean destroy

all: _provision.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok

_init.ok:
	terraform init servers
	@touch $@

_server.ok: _init.ok _id_ed25519
	terraform apply -auto-approve servers
	@touch $@

_provision.ok: _server.ok _hosts _known_hosts provision.yml
	ansible-playbook provision.yml
	@touch $@

_id_ed25519:
	ssh-keygen -t ed25519 -N "" -C "tempkey" -f $@

_ssh_config: _server.ok
	terraform output ssh_config > $@

_hosts: _server.ok
	terraform output ansible_inventory > $@

_known_hosts: _ssh_config
	ssh -F _ssh_config srv :
	ssh -F _ssh_config pen :
