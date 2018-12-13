ARTIFACTS =                \
  terraform.tfstate        \
  terraform.tfstate.backup \
  _init.ok                 \
  _resources.ok            \
  _provision.ok            \
  ssh_config               \
  known_hosts              \
  provision/bucket.service

TF_OPTIONS = \
  -var-file config.json

SED_RULES = \
  -e "s/{{storage_bucket}}/$$(jq '.storage_bucket' < config.json)/g"


.PHONY: all clean ssh

all: _provision.ok
	@:

clean:
	terraform destroy -auto-approve $(TF_OPTIONS)
	rm -rf $(ARTIFACTS)

ssh: ssh_config
	ssh -F ssh_config server

_init.ok:
	terraform init $(TF_INIT_OPTIONS)
	@touch $@

_resources.ok: _init.ok
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@

_provision.ok: ssh_config provision/bucket.service
	tar cf - provision | ssh -F ssh_config server "tar xf - && bash provision/run.bash"
	@touch $@

ssh_config: _resources.ok ssh_config.in
	terraform output ssh_config > $@

provision/bucket.service: provision/bucket.service.in
	sed $(SED_RULES) $< > $@
