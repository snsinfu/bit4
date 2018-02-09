package clio

import (
	"compress/gzip"
	"io"
	"os"
	"strings"
)

const (
	gzipSuffix = ".gz"
	stdinName  = "-"
	stdoutName = "-"
)

func Open(name string) (io.ReadCloser, error) {
	rd, err := open(name)
	if err != nil {
		return nil, err
	}

	if strings.HasSuffix(name, gzipSuffix) {
		gz, err := gzip.NewReader(rd)
		if err != nil {
			rd.Close()
			return nil, err
		}
		rd = gz
	}
	return rd, nil
}

func open(name string) (io.ReadCloser, error) {
	if name == stdinName {
		return &readNoCloser{os.Stdin}, nil
	}
	return os.Open(name)
}

func Create(name string) (io.WriteCloser, error) {
	wr, err := create(name)
	if err != nil {
		return nil, err
	}

	if strings.HasSuffix(name, gzipSuffix) {
		wr = gzip.NewWriter(wr)
	}
	return wr, nil
}

func create(name string) (io.WriteCloser, error) {
	if name == stdoutName {
		return &writeNoCloser{os.Stdout}, nil
	}
	return os.Create(name)
}
