TEMPLATE = geo_template.hpp
ROOT_HEADER = geo/all.hpp

.PHONY: clean

geo.hpp:
	python3 bundle.py --template $(TEMPLATE) $(ROOT_HEADER) > $@ || rm $@

clean:
	rm -f geo.hpp
