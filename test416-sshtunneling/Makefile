ARTIFACTS =            \
  sshkey-forwarder     \
  sshkey-forwarder.pub \
  vagrant-ssh_config   \
  provision.retry      \
  _provision.ok        \
  _machine.ok

.PHONY: forward clean

_provision.ok: provision.yml sshkey-forwarder _machine.ok
	vagrant provision
	@touch $@

_machine.ok: Vagrantfile
	vagrant up --no-provision
	@touch $@

vagrant-ssh_config: _machine.ok
	vagrant ssh-config | egrep -v '\b(User|IdentityFile)\b' > $@

sshkey-forwarder:
	ssh-keygen -f $@ -N ""

forward: vagrant-ssh_config sshkey-forwarder _provision.ok
	ssh -v -N -R 10080::3000 -F vagrant-ssh_config -i sshkey-forwarder forwarder@default

clean:
	vagrant destroy -f
	rm -f $(ARTIFACTS)
