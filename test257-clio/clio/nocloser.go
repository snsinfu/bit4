package clio

import (
	"io"
)

type readNoCloser struct {
	rd io.Reader
}

func (r *readNoCloser) Read(buf []byte) (int, error) {
	return r.rd.Read(buf)
}

func (r *readNoCloser) Close() error {
	return nil
}

type writeNoCloser struct {
	wr io.Writer
}

func (w *writeNoCloser) Write(buf []byte) (int, error) {
	return w.wr.Write(buf)
}

func (w *writeNoCloser) Close() error {
	return nil
}
