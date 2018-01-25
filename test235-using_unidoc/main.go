package main

import (
	"fmt"
	"os"
	"strings"

	pdfcontent "github.com/unidoc/unidoc/pdf/contentstream"
	pdfcore "github.com/unidoc/unidoc/pdf/core"
	pdf "github.com/unidoc/unidoc/pdf/model"
)

func main() {
	for _, path := range os.Args[1:] {
		if err := guessDOI(path); err != nil {
			fmt.Fprintln(os.Stderr, "error: ", err)
			os.Exit(1)
		}
	}
}

func guessDOI(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	reader, err := pdf.NewPdfReader(f)
	if err != nil {
		return err
	}

	pageCount, err := reader.GetNumPages()

	for pageNum := 1; pageNum <= pageCount; pageNum++ {
		page, err := reader.GetPage(pageNum)
		if err != nil {
			return err
		}

		if err := lookupDOI(page); err != nil {
			return err
		}
	}

	return nil
}

func lookupDOI(page *pdf.PdfPage) error {
	content, err := page.GetAllContentStreams()
	if err != nil {
		return err
	}

	parser := pdfcontent.NewContentStreamParser(content)
	ops, err := parser.Parse()
	if err != nil {
		return err
	}

	for _, op := range *ops {
		if isTextOperation(op) {
			val, ok := op.Params[0].(*pdfcore.PdfObjectString)
			if ok {
				str := string(*val)
				if strings.Contains(str, "doi") {
					fmt.Println(str)
				}
			}
		}
	}

	return nil
}

func isTextOperation(op *pdfcontent.ContentStreamOperation) bool {
	return op.Operand == "Tj" && len(op.Params) == 1
}
