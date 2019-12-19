CXXFLAGS = \
  -std=c++14

.PHONY: run clean

run: config.py main
	python main.py
	./main

clean:
	rm -f config.py main *.pyc

config.py: config.py.in
	$(CPP) $(CPPFLAGS) -xc -P -o $@ $<

config.py: config_members.hpp
