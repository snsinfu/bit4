PRODUCTS = tbf05.json
CHECKPOINTS = _pipenv.ok

.PHONY: all clean
.SUFFIXES: .want .json

all: $(CHECKPOINTS) $(PRODUCTS)
	@:

clean:
	pipenv --rm || :
	rm -f $(CHECKPOINTS)

.want.json:
	pipenv run python scrape_circle_info.py $$(cat $<) > $@ || rm $@

_pipenv.ok:
	pipenv install
	@touch $@
