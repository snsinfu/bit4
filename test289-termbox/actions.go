package main

type Action struct {
	Type ActionType
	Ch   rune
	Err  error
}

type ActionType int

const (
	ActQuit ActionType = iota
	ActInputChar
	ActPutChar
	ActAdvanceCursor
)
