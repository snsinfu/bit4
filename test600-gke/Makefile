ZONE = us-central1-a
CLUSTER_NAME = main
CLUSTER_OPTIONS = --machine-type=g1-small --num-nodes=1 --preemptible

CHECKPOINTS = \
  _zone.ok \
  _cluster.ok \
  _credentials.ok \
  _deploy.ok \
  _service.ok \
  _firewall.ok \
  _address.ok


.PHONY: test clean

test: _service.ok _firewall.ok _address.ok
	curl -L "http://$$(cat _address.ok):30080/"

clean:
	test -f _service.ok && kubectl delete -f hello-service.yaml && rm _service.ok
	test -f _deploy.ok && kubectl delete -f hello-deploy.yaml && rm _deploy.ok
	test -f _firewall.ok && gcloud compute firewall-rules -q delete nodeport-http && rm _firewall.ok
	test -f _cluster.ok && gcloud container clusters delete -q $(CLUSTER_NAME) && rm _cluster.ok
	rm -f $(CHECKPOINTS)

_zone.ok:
	gcloud config set compute/zone $(ZONE)
	@touch $@

_cluster.ok: _zone.ok
	gcloud container clusters create $(CLUSTER_OPTIONS) $(CLUSTER_NAME)
	@touch $@

_firewall.ok: _cluster.ok
	gcloud compute firewall-rules create nodeport-http --allow tcp:30080
	@touch $@

_address.ok: _cluster.ok
	gcloud compute instances list | awk 'NR == 2 { print $$6 }' > $@

_credentials.ok: _cluster.ok
	gcloud container clusters get-credentials $(CLUSTER_NAME)
	@touch $@

_deploy.ok: _cluster.ok _credentials.ok hello-deploy.yaml
	kubectl create -f hello-deploy.yaml
	@touch $@

_service.ok: _deploy.ok hello-service.yaml
	kubectl create -f hello-service.yaml
	@touch $@
