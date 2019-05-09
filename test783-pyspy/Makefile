ENV = test783-pyspy
PYSPY = $$(test $$(uname) = Darwin && echo sudo) py-spy

.PHONY: run clean

run: _env.ok
	source activate $(ENV); $(PYSPY) python main.py

clean:
	conda env remove -n $(ENV)
	rm -f _env.ok

_env.ok:
	conda env create -n $(ENV) -f environment.yml
	touch $@
