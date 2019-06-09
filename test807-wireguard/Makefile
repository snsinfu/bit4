ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  provision.retry \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _id_ed25519 \
  _id_ed25519.pub \
  _ssh_config \
  _hosts \
  output

TF_OPTIONS = \
  -var-file .vars.json


.PHONY: all clean destroy provision reboot ssh

all: _server.ok
	@:

clean:
	rm -rf $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve $(TF_OPTIONS)
	rm -f _server.ok

provision: _provision.ok
	@:

reboot: _ssh_config
	ssh -F _ssh_config vpnserver "sudo reboot"

ssh: _ssh_config
	ssh -F _ssh_config vpnserver

_init.ok: *.tf
	terraform init
	@touch $@

_server.ok: _init.ok _id_ed25519
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@

_provision.ok: provision.yml roles/**/* host_vars/*.yml _hosts
	ansible-playbook provision.yml
	@touch $@

_ssh_config: tf_templates/ssh_config.in _server.ok
	terraform output ssh_config > $@

_hosts: tf_templates/hosts.in _server.ok
	terraform output hosts > $@

_id_ed25519:
	ssh-keygen -t ed25519 -N "" -f $@
