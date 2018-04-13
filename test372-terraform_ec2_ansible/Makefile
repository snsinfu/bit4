ARTIFACTS =                \
  terraform.tfstate        \
  terraform.tfstate.backup \
  id_rsa                   \
  id_rsa.pub

.PHONY: provision clean

provision: provision.yml id_rsa terraform.tfstate
	ansible-playbook -i $$(which terraform-inventory) --private-key id_rsa provision.yml

clean:
	terraform destroy
	rm -rf $(ARTIFACTS)

terraform.tfstate: main.tf .terraform
	terraform apply

.terraform:
	terraform init

id_rsa:
	ssh-keygen -t rsa -b 4096 -f $@ -N ""
