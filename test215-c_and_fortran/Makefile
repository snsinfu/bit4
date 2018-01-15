OBJECTS = driver.o kernel.o
TARGET = ./run

FC = gfortran
FFLAGS = -std=f2003 -ffree-form
LDFLAGS = -lgfortran -L/usr/local/lib/gcc/7

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
