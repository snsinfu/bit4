CHECKPOINTS = \
  init.ok \
  server.ok

TF_ARTIFACTS = \
  .terraform \
  terraform.tfstate \
  terraform.tfstate.backup

GENERATED_FILES = \
  manifest.json \
  image_id.txt \
  id_rsa \
  id_rsa.pub \
  known_hosts

ARTIFACTS = \
  $(CHECKPOINTS) \
  $(TF_ARTIFACTS) \
  $(GENERATED_FILES)

SSH_OPTIONS = \
  -o UserKnownHostsFile=known_hosts \
  -o PasswordAuthentication=no \
  -o ChallengeResponseAuthentication=no


.PHONY: all clean ssh

all: server.ok
	@:

clean:
	TF_VAR_image_id=$$(cat image_id.txt) terraform destroy -auto-approve
	./hcloud_remove_image.sh $$(cat image_id.txt)
	rm -rf $(ARTIFACTS)

ssh: server.ok id_rsa
	ssh $(SSH_OPTIONS) -i id_rsa root@$$(terraform output ip)

manifest.json:
	packer build main.json

image_id.txt: manifest.json
	jq -r ".builds[0].artifact_id" < manifest.json > $@

init.ok:
	terraform init
	@touch $@

server.ok: init.ok image_id.txt id_rsa
	TF_VAR_image_id=$$(cat image_id.txt) terraform apply -auto-approve
	@touch $@

id_rsa:
	ssh-keygen -t ed25519 -f $@ -N "" -C ""
