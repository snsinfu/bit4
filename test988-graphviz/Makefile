PRODUCTS = \
  graph.png \
  graph.svg

DOTFLAGS = \
  $(USE_GOTHIC) \
  $(USE_RECT)

USE_GOTHIC = \
  -Gfontname=helvetica \
  -Nfontname=helvetica \
  -Efontname=helvetica \
  -Gfontsize=10 \
  -Nfontsize=10 \
  -Efontsize=10

USE_RECT = \
  -Gsplines=ortho \
  -Nshape=box


.PHONY: all clean
.SUFFIXES: .dot .png .svg

all: $(PRODUCTS)
	@:

clean:
	rm -f $(PRODUCTS)

.dot.png:
	dot $(DOTFLAGS) -Tpng -o$@ $^

.dot.svg:
	dot $(DOTFLAGS) -Tsvg -o$@ $^
