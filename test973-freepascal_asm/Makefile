FPC = fpc

FPCFLAGS = \
  -vw \
  -O3

TARGET = main
ARTIFACTS = $(TARGET) *.o *.ppu


.PHONY: all clean

all: $(TARGET)
	@:

clean:
	rm -f $(ARTIFACTS)

main: main.pas
	$(FPC) $(FPCFLAGS) -o$@ $^
