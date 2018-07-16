package main

type Task interface {
	Run() error
}

type PrintTask struct {
	Message string
}

func (task PrintTask) Run() error {
	return nil
}

func main() {
	doit(nil)
}

func doit(task Task) {
	if pt, ok := task.(*PrintTask); ok {
		pt.Message = ""
	}
}
