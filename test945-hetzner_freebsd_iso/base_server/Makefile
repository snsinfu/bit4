ARTIFACTS = \
  _init.ok \
  _server.ok \
  _terraform.tfstate \
  _terraform.tfstate.backup


.PHONY: all clean destroy

all: _server.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm _server.ok

_init.ok:
	terraform init
	touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	touch $@
