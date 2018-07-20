ARTIFACTS =                \
  ssh_key                  \
  ssh_key.pub              \
  terraform.tfstate        \
  terraform.tfstate.backup \
  provision.retry          \
  _server.ok               \
  _init.ok

SSH_OPTIONS =                     \
  -o StrictHostKeyChecking=no     \
  -o UserKnownHostsFile=/dev/null \
  -o PasswordAuthentication=no    \
  -o IdentitiesOnly=yes

.PHONY: clean ssh

_provision.ok: provision.yml ssh_key _server.ok
	ansible-playbook provision.yml
	@touch $@

_server.ok: server.tf ssh_key _init.ok
	terraform apply -auto-approve
	@touch $@

_init.ok:
	terraform init
	@touch $@

ssh_key:
	ssh-keygen -f $@ -N ""

clean:
	terraform destroy -force
	rm -f $(ARTIFACTS)

ssh: ssh_key _provision.ok
	ssh $(SSH_OPTIONS) -i ssh_key auser@$$(terraform output ip)
