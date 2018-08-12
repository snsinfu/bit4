APPFILES =         \
  requirements.txt \
  main.py

ARTIFACTS =     \
  __pycache__   \
  .pytest_cache \
  .venv         \
  _venv.ok      \
  _deps.ok      \
  _app.ok

.PHONY: deploy clean fmt test debug

deploy: _app.ok
	@:

clean:
	rm -fr $(ARTIFACTS)

fmt:
	black .

test: _deps.ok
	.venv/bin/pytest

debug: _deps.ok
	.venv/bin/python -m sanic --debug --host 127.0.0.1 --port 8080 main.app

_app.ok: app.yaml $(APPFILES)
	gcloud app deploy -q
	@touch $@

_deps.ok: _venv.ok requirements.txt requirements-dev.txt
	.venv/bin/pip install -r requirements.txt -r requirements-dev.txt
	@touch $@

_venv.ok:
	python3 -m venv .venv
	@touch $@
