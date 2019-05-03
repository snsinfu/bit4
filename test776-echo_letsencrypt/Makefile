ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  provision.retry \
  _init.ok \
  _server.ok \
  _provision.ok \
  _known_hosts \
  _ssh_config \
  _hosts \
  _server_app \
  _start_server_app.bash

TF_OPTIONS = \
  -var-file .vars.json


.PHONY: all clean destroy ssh

all: _provision.ok
	@:

clean:
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve $(TF_OPTIONS)

test: _provision.ok
	curl https://$$(terraform output hostname)/

start: _ssh_config
	ssh -F _ssh_config test-machine ./start_server_app.bash

ssh: _ssh_config
	ssh -F _ssh_config test-machine

_init.ok: *.tf
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@

_provision.ok: provision.yml _hosts _server_app _start_server_app.bash
	ansible-playbook provision.yml
	@touch $@

_ssh_config: ssh_config.in _server.ok
	terraform output ssh_config > $@

_hosts: hosts.in _server.ok
	terraform output hosts > $@

_server_app: server_app/main.go
	GOOS=linux GOARCH=amd64 go build -o $@ ./server_app

_start_server_app.bash: start_server_app.bash.in
	terraform output start_server_app > $@
