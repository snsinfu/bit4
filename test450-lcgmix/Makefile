OPTFLAGS = \
  -O2

CXXFLAGS =          \
  -std=c++1z        \
  -pedantic         \
  -Wall             \
  -Wextra           \
  -Wconversion      \
  -Wsign-conversion \
  -Wshadow          \
  $(OPTFLAGS)

ARTIFACTS = \
  rand      \
  rand.dat

.SUIFFIXES: .cc
.PHONY: clean

rand.dat: rand
	./rand > $@

clean:
	rm -f $(ARTIFACTS)

.cc:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)
