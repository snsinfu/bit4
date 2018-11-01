PRE = bundle_pre.hpp

HEADERS = \
  geo/basic_types.hpp \
  geo/objects/point.hpp \
  geo/objects/vector.hpp

.PHONY: clean

geo.hpp: $(PRE) $(HEADERS)
	python3 bundle.py --pre $(PRE) --guard GEO_HPP $(HEADERS) > $@ || rm $@

clean:
	rm -f geo.hpp
