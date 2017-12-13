SOURCES = main.go hsv.go
SAMPLES = \
 samples/01.png samples/02.png samples/03.png \
 samples/04.png samples/05.png samples/06.png \
 samples/07.png samples/08.png samples/09.png \
 samples/10.png samples/11.png samples/12.png

.PHONY:    test samples clean
.SUFFIXES: .png .txt

identicon: $(SOURCES)
	go build -o $@

test:
	go test

.txt.png:
	./identicon "$$(cat $<)" $@

samples: identicon $(SAMPLES)
	@:

clean:
	rm -f identicon $(SAMPLES)
