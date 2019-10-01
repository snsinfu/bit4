ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _init.ok \
  _server.ok \
  _known_hosts \
  _ssh_config

.PHONY: all clean destroy

all: _server.ok _ssh_config
	@:

clean: destroy
	rm -rf $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve -var-file .vars.json
	@rm -f _server.ok

_init.ok:
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve -var-file .vars.json
	@touch $@

_ssh_config: _server.ok
	terraform output ssh_config > $@
