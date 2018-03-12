CXXFLAGS = $(PROTOBUF_CXXFLAGS) -std=c++14 -pedantic -Wall -Wextra
LDFLAGS = $(PROTOBUF_LDFLAGS)

PROTOBUF_CXXFLAGS = $$(pkg-config --cflags protobuf)
PROTOBUF_LDFLAGS = $$(pkg-config --libs protobuf)

OBJECTS = main.o profile.pb.o

.PHONY: clean

test.pb: main
	./main $@

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

main.cc: profile.pb.h

profile.proto:
	curl -fsSLO "https://github.com/google/pprof/raw/master/proto/$@"

%.pb.h %.pb.cc: %.proto
	protoc --cpp_out=. $<

clean:
	rm -f main $(OBJECTS) *.pb.h *.pb.cc test.pb
