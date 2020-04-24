ARTIFACTS = \
  _init.ok \
  _resources.ok \
  _ssh_config \
  _known_hosts


.PHONY: all destroy clean ssh

all: _ssh_config
	@:

destroy:
	terraform destroy -auto-approve

clean:
	rm -f $(ARTIFACTS)

ssh: _ssh_config
	ssh -F _ssh_config main

_init.ok:
	terraform init
	@touch $@

_resources.ok: _init.ok
	terraform apply -auto-approve
	@touch $@

_ssh_config: _resources.ok
	terraform output ssh_config > $@
