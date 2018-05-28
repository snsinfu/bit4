ARTIFACTS =                \
  terraform.tfstate        \
  terraform.tfstate.backup \
  ping.retry               \
  provision.retry          \
  id_rsa                   \
  id_rsa.pub               \
  _init.ok                 \
  _apply.ok                \
  _provision.ok

SSH_OPTIONS =                 \
  -o StrictHostKeyChecking=no \
  -o IdentitiesOnly=yes       \
  -o PasswordAuthentication=no

.PHONY: ssh mosh clean

_provision.ok: provision.yml _apply.ok id_rsa
	ansible-playbook provision.yml
	touch $@

_apply.ok: main.tf _init.ok id_rsa
	terraform apply -auto-approve
	touch $@

_init.ok:
	terraform init
	touch $@

id_rsa:
	ssh-keygen -t rsa -b 4096 -f $@ -N ""

ssh: id_rsa _provision.ok
	ssh $(SSH_OPTIONS) -i id_rsa user@$$(terraform output ip)

mosh: id_rsa _provision.ok
	mosh --ssh='ssh $(SSH_OPTIONS) -i id_rsa' user@$$(terraform output ip)

clean:
	terraform destroy -force
	rm -f $(ARTIFACTS)
