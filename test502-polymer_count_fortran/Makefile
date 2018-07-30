PRODUCTS = main

FC = gfortran

OPTFLAGS = \
  -O2

FFLAGS =      \
  -std=f2008  \
  -pedantic   \
  -Wall       \
  -ffree-form \
  $(OPTFLAGS)

.PHONY: all clean

all: $(PRODUCTS)
	@$:

clean:
	rm -f $(PRODUCTS)

main: main.f08
	$(FC) $(FFLAGS) $(LDFLAGS) -o $@ main.f08
