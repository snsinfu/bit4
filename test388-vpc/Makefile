ARTIFACTS =         \
  ssh_config        \
  id_rsa            \
  id_rsa.pub        \
  init.done         \
  apply.done        \
  terraform.tfstate \
  terraform.tfstate.backup

.PHONY: clean

ssh_config: ssh_config.in apply.done
	@sed "/^HostName/ s/@/$$(terraform output ip)/" ssh_config.in | tee $@

apply.done: init.done id_rsa main.tf
	terraform apply -auto-approve
	touch $@

init.done:
	terraform init
	touch $@

id_rsa:
	ssh-keygen -t rsa -b 4096 -f $@ -N ""

clean:
	terraform destroy -auto-approve
	rm -f $(ARTIFACTS)
