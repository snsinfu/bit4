ARTIFACTS = \
  .terraform \
  terraform.tfstate \
  terraform.tfstate.backup \
  known_hosts \
  _init.ok \
  _server.ok

SSH_OPTIONS = \
  -o UserKnownHostsFile=known_hosts \
  -o PasswordAuthentication=no \
  -o IdentitiesOnly=yes \
  -i ~/.ssh/id_rsa \
  -l root

.PHONY: all clean

all: _server.ok
	@:

clean:
	terraform destroy -auto-approve
	rm -rf $(ARTIFACTS)

ssh: _server.ok
	ssh $(SSH_OPTIONS) $$(terraform output ip)

_init.ok:
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	@touch $@
