ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  provision.retry \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _ssh_config \
  _hosts

TF_OPTIONS = \
  -var-file .vars.json


.PHONY: all clean destroy ssh

all: _provision.ok
	@:

clean:
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve $(TF_OPTIONS)

ssh: _ssh_config
	ssh -F _ssh_config test-machine

_init.ok: *.tf
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@

_provision.ok: provision.yml _hosts
	ansible-playbook provision.yml
	@touch $@

_ssh_config: templates/ssh_config.in _server.ok
	terraform output ssh_config > $@

_hosts: templates/hosts.in _server.ok
	terraform output hosts > $@
