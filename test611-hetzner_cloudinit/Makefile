ARTIFACTS = \
  _init.ok \
  _server.ok \
  terraform.tfstate \
  terraform.tfstate.backup \
  provision.retry \
  known_hosts \
  id_rsa \
  id_rsa.pub

SSH_OPTIONS = \
  -o UserKnownHostsFile=known_hosts \
  -o IdentitiesOnly=yes \
  -o PasswordAuthentication=no


.PHONY: all clean ssh

all: _server.ok
	@:

clean:
	terraform destroy -auto-approve
	rm -rf $(ARTIFACTS)

ssh: _server.ok id_rsa
	ssh $(SSH_OPTIONS) -i id_rsa root@$$(terraform output ip)

_init.ok: main.tf
	terraform init
	@touch $@

_server.ok: _init.ok main.tf vars.tf id_rsa
	terraform apply -auto-approve
	@touch $@

id_rsa:
	ssh-keygen -t ed25519 -f $@ -N ""
