APPFILES =         \
  requirements.txt \
  main.py          \
  templates/home.html.j2

ARTIFACTS =   \
  __pycache__ \
  .venv       \
  _deps.ok    \
  _venv.ok    \
  _app.ok     \
  _index.ok   \
  _cron.ok

.PHONY: nop deploy clean fmt test

nop:
	@:

deploy: _app.ok _index.ok _cron.ok
	@:

clean:
	rm -fr $(ARTIFACTS)

fmt:
	black .

test: _deps.ok
	.venv/bin/python -m unittest -v

_app.ok: app.yaml $(APPFILES)
	gcloud app deploy -q
	@touch $@

_index.ok: index.yaml
	gcloud app deploy -q index.yaml
	@touch $@

_cron.ok: cron.yaml
	gcloud app deploy -q cron.yaml
	@touch $@

_deps.ok: _venv.ok requirements.txt
	.venv/bin/pip install -r requirements.txt
	@touch $@

_venv.ok:
	python3 -m venv .venv
	@touch $@
