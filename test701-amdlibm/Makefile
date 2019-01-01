WARNINGS = \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow

OPTFLAGS = \
  -O2 \
  -flto

CFLAGS = -std=c99 $(WARNINGS) $(OPTFLAGS)
CXXFLAGS = -std=c++17 $(WARNINGS) $(OPTFLAGS)

ARTIFACTS = \
  main \
  $(OBJECTS)

OBJECTS = \
  main.o \
  libm.o \

STATIC_LIBS = \
  libamdlibm.a


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(STATIC_LIBS)
