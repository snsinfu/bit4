WXLIBS = core,base

CXXFLAGS = \
  -std=c++17 \
  -Wpedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -O2 \
  $$(wx-config --cxxflags | sed "s/-I/-isystem/g")

LDFLAGS = \
  $$(wx-config --libs $(WXLIBS))

OBJECTS = \
  App.o \
  MainWindow.o


.PHONY: all clean
.SUFFIXES: .cpp

all: main
	@:

clean:
	rm -f main $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
