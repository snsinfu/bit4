package main

import (
	"fmt"
	"os"

	"github.com/fsnotify/fsnotify"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

func run() error {
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		return err
	}
	defer watcher.Close()

	if err := watcher.Add("target"); err != nil {
		return err
	}

	for event := range watcher.Events {
		if event.Op == fsnotify.Create {
			fmt.Println("New file:", event.Name)
		}
	}
	return nil
}
