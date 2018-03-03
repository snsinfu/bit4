package main

var actions chan Action

func init() {
	actions = make(chan Action, 16)
}

func dispatch(act Action) {
	actions <- act
}
