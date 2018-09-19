OPTFLAGS = \
  -O2      \
  -flto

CXXFLAGS =          \
  -std=c++11        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  -std=c++11        \
  $(OPTFLAGS)

ARTIFACTS = nbow

.SUFFIXES: .cc
.PHONY: clean

nbow: nbow.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ nbow.cc

clean:
	rm -f $(ARTIFACTS)
