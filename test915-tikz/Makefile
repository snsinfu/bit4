ARTIFACTS = main.pdf main.aux main.log

.PHONY: all clean
.SUFFIXES: .tex .pdf

all: main.pdf
	@:

clean:
	rm -f $(ARTIFACTS)

.tex.pdf:
	pdflatex $<
