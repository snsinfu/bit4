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


.PHONY: all clean destroy provision client ssh

all: _server.ok
	@:

clean:
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve $(TF_OPTIONS)
	rm -f _server.ok

provision: _provision.ok
	@:

client: _provision.ok
	ansible-playbook make_client.yml

ssh: _ssh_config
	ssh -F _ssh_config server

_init.ok: *.tf
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@

_provision.ok: provision.yml _hosts
	ansible-playbook provision.yml
	@touch $@

_ssh_config: ssh_config.in _server.ok
	terraform output ssh_config > $@

_hosts: hosts.in _server.ok
	terraform output hosts > $@
