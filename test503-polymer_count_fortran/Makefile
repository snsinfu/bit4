PRODUCTS = main

FC = gfortran

OPTFLAGS = \
  -O2

FFLAGS =       \
  -std=f95     \
  -ffixed-form \
  -pedantic    \
  -Wall        \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCTS)
	@$:

clean:
	rm -f $(PRODUCTS)

main: main.f
	$(FC) $(FFLAGS) $(LDFLAGS) -o $@ main.f
