# Requires GNU ld

PRODUCT = main
OBJECTS = main.o embed.o
ARTIFACTS = $(PRODUCT) $(OBJECTS)

CFLAGS =            \
  -std=c99          \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow

.PHONY: run clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(ARTIFACTS)

embed.o: embed.dat
	ld -r -b binary -o $@ embed.dat

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
