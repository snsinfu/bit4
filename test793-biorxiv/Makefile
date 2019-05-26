BASE_PYTHON = python3
VENV_PYTHON = _venv/bin/python

ARTIFACTS = \
  _venv \
  _venv.ok


.PHONY: run clean

run: _venv.ok
	@$(VENV_PYTHON) main.py

clean:
	rm -rf $(ARTIFACTS)

_venv.ok: requirements.txt
	rm -rf _venv
	$(BASE_PYTHON) -m venv _venv
	$(VENV_PYTHON) -m pip install -r requirements.txt
	@touch $@
