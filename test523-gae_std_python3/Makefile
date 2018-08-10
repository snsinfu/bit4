ARTIFACTS =   \
  __pycache__ \
  .venv       \
  _deps.ok    \
  _venv.ok

.PHONY: nop deploy clean fmt test

nop:
	@:

deploy: app.yaml
	gcloud app deploy

clean:
	rm -fr $(ARTIFACTS)

fmt:
	black .

test: _deps.ok
	.venv/bin/python -m unittest -v

_deps.ok: _venv.ok requirements.txt
	.venv/bin/pip install -r requirements.txt
	@touch $@

_venv.ok:
	python3 -m venv .venv
	@touch $@
