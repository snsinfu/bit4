package qstat

import (
	"reflect"
	"strconv"
)

const (
	TagKey = "qstat"
)

type Job struct {
	Name              string `qstat:"Job_Name"`
	Owner             string `qstat:"Job_Owner"`
	State             string `qstat:"job_state"`
	Queue             string `qstat:"queue"`
	Server            string `qstat:"server"`
	CreationTime      string `qstat:"ctime"`
	QueueTime         string `qstat:"qtime"`
	ModificationTime  string `qstat:"mtime"`
	InitialWorkDir    string `qstat:"init_work_dir"`
	OutputPath        string `qstat:"Output_Path"`
	ErrorPath         string `qstat:"Error_Path"`
	ExecutionHost     string `qstat:"exec_host"`
	ExecutionUser     string `qstat:"euser"`
	ExecutionGroup    string `qstat:"egroup"`
	SubmissionHost    string `qstat:"submit_host"`
	SessionID         int    `qstat:"session_id"`
	ReservedNodes     string `qstat:"Resource_List.nodes"`
	ReservedNodeCount int    `qstat:"Resource_List.nodect"`
	ReservedWalltime  string `qstat:"Resource_List.walltime"`
	UsedWalltime      string `qstat:"resources_used.walltime"`
	UsedCPUTime       string `qstat:"resources_used.cput"`
	UsedMemory        string `qstat:"resources_used.mem"`
	UsedVirtualMemory string `qstat:"resources_used.vmem"`
	RemainingWalltime string `qstat:"Walltime.Remaining"`
	Variables         string `qstat:"Variable_List"`
}

// LoadJob parses attributes in a raw record and creates a Job structure.
func LoadJob(rec RawRecord) (Job, error) {
	job := Job{}
	val := valueRef(&job)
	typ := val.Type()

	for i := 0; i < typ.NumField(); i++ {
		tag := typ.Field(i).Tag.Get(TagKey)

		str, ok := rec[tag]
		if !ok {
			continue
		}

		if err := loadAttr(val.Field(i), str); err != nil {
			return job, err
		}
	}
	return job, nil
}

// valueRef creates a pointed-to Value of a pointer.
func valueRef(ptr interface{}) reflect.Value {
	if reflect.TypeOf(ptr).Kind() != reflect.Ptr {
		panic("expected a pointer")
	}
	return reflect.Indirect(reflect.ValueOf(ptr))
}

// loadAttr parses a string and sets a value to a Job field variable.
func loadAttr(val reflect.Value, str string) error {
	switch val.Interface().(type) {
	case string:
		val.SetString(str)

	case int:
		num, err := strconv.ParseInt(str, 10, 64)
		if err != nil {
			return err
		}
		val.SetInt(num)
	}
	return nil
}
