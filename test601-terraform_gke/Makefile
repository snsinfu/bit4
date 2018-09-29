ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _init.ok \
  _apply.ok

.PHONY: test clean

test: _apply.ok
	curl -L "http://$$(gcloud compute instances list | awk 'NR == 2 { print $$6 }'):30080/"

clean:
	terraform destroy -force
	rm -rf $(ARTIFACTS)

_init.ok: main.tf
	terraform init
	@touch $@

_apply.ok: main.tf _init.ok
	terraform apply -auto-approve
	@touch $@
