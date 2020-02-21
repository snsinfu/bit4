CFLAGS = \
  -std=c99 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(DEFINES) \
  $(OPTFLAGS)

DEFINES = \
  -D_POSIX_C_SOURCE=200112L

OPTFLAGS = \
  -O2 \
  -march=native

PRODUCTS = \
  bench_mul \
  bench_pow \
  bench_intpow \
  bench_optimized

MUL_OBJECTS = main.o mul.o
POW_OBJECTS = main.o pow.o
INTPOW_OBJECTS = main.o intpow.o
OPTIMIZED_OBJECTS = main.o optimized.o

ARTIFACTS = \
  $(PRODUCTS) \
  $(MUL_OBJECTS) \
  $(POW_OBJECTS) \
  $(INTPOW_OBJECTS) \
  $(OPTIMIZED_OBJECTS)


.PHONY: all clean

all: $(PRODUCTS)
	@:

clean:
	rm -f $(ARTIFACTS)

bench_mul: $(MUL_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bench_pow: $(POW_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bench_intpow: $(INTPOW_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bench_optimized: $(OPTIMIZED_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
