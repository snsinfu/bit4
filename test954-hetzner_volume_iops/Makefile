ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _init.ok \
  _server.ok \
  _ssh_config \
  _known_hosts


.PHONY: all clean destroy ssh

all: _ssh_config
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok

ssh: _ssh_config
	ssh -F _ssh_config main

_init.ok:
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	@touch $@

_ssh_config: _server.ok
	terraform output ssh_config > $@
