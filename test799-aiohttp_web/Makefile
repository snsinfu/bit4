BASE_PYTHON = python3
VENV_PYTHON = _venv/bin/python


.PHONY: run clean format

run: _venv
	$(VENV_PYTHON) main.py

clean:
	rm -rf _venv

format:
	black --exclude _venv .

_venv: requirements.txt
	rm -rf $@
	$(BASE_PYTHON) -m venv $@
	$(VENV_PYTHON) -m pip install -r requirements.txt
