TF_OPTIONS = -var-file config.json

ARTIFACTS =                \
  terraform.tfstate        \
  terraform.tfstate.backup \
  _init.ok                 \
  _resources.ok

.PHONY: all clean

all: _resources.ok
	@:

clean:
	terraform destroy -auto-approve $(TF_OPTIONS)
	rm -rf $(ARTIFACTS)

_init.ok:
	terraform init
	@touch $@

_resources.ok: _init.ok
	terraform apply -auto-approve $(TF_OPTIONS)
	@touch $@
