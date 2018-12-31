INCLUDES = \
  -I include

OPTFLAGS = \
  -O2

CXXFLAGS = \
  -std=c++17 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow \
  $(INCLUDES) \
  $(OPTFLAGS)

ARTIFACTS = \
  main \
  $(OBJECTS)

OBJECTS = \
  main.o \
  sub.o


.PHONY: all clean

all: main
	@:

clean:
	rm -f $(ARTIFACTS)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)
