TEMPLATE = geo_template.hpp

HEADERS = \
  geo/basic_types.hpp \
  geo/objects/point.hpp \
  geo/objects/vector.hpp

.PHONY: clean

geo.hpp: $(TEMPLATE) $(HEADERS)
	python3 bundle.py --template $(TEMPLATE) $(HEADERS) > $@ || rm $@

clean:
	rm -f geo.hpp
