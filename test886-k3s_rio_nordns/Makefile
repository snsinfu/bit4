ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _ssh_config \
  inventory/_terraform_hosts \
  _k3s.yaml

.PHONY: all clean destroy ssh

all: _provision.ok _ssh_config
	@:

clean: destroy
	rm -rf $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve -var-file vars.json
	@rm -f _server.ok

ssh: _ssh_config
	ssh -F _ssh_config master

_init.ok:
	terraform init -var-file vars.json
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve -var-file vars.json
	@touch $@

_provision.ok: _server.ok inventory/_terraform_hosts provision.yml
	ansible-playbook provision.yml
	@touch $@

inventory/_terraform_hosts: _server.ok
	terraform output ansible_inventory > $@

_ssh_config: _server.ok
	terraform output ssh_config > $@
