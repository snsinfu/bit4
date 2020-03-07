PRODUCTS = \
  topix.tsv \
  nikkei225.tsv \
  jreit.tsv \
  jp_bond.tsv \
  us_bond.tsv \
  us_bond_hedge.tsv


.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

nikkei225.tsv:     ; node index.js 251897 > $@
jreit.tsv:         ; node index.js 279435 > $@
topix.tsv:         ; node index.js 279438 > $@
us_bond_hedge.tsv: ; node index.js 283198 > $@
us_bond.tsv:       ; node index.js 290497 > $@
jp_bond.tsv:       ; node index.js 313081 > $@
