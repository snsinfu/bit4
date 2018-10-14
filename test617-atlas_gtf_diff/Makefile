PRODUCT = main

WARNFLAGS = \
  -std=c++11 \
  -pedantic \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  -Wshadow

OPTFLAGS = \
  -O2

CXXFLAGS = \
  $(WARNFLAGS) \
  $(OPTFLAGS) \
  $(EXTRA_CXXFLAGS)

.PHONY: all clean

all: $(PRODUCT)
	@:

clean:
	rm -f $(PRODUCT)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

main: gtf.hpp strings.hpp
