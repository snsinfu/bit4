package main

// Netstat workalike. For some reason gopsutil/net needs root privilege to get
// pid of network-communicating process. It should not.

import (
	"fmt"
	"os"
	"os/user"
	"strconv"

	"github.com/shirou/gopsutil/net"
	"github.com/shirou/gopsutil/process"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	conns, err := net.Connections("tcp")
	if err != nil {
		return err
	}

	for _, conn := range conns {
		if conn.Status == "LISTEN" {
			proc, err := process.NewProcess(conn.Pid)
			if err != nil {
				continue
			}
			cmd, _ := proc.Cmdline()

			users := map[string]int{}
			for _, uid := range conn.Uids {
				u, err := user.LookupId(strconv.Itoa(int(uid)))
				if err != nil {
					continue
				}
				users[u.Name] = 1
			}

			data := map[string]interface{}{
				"addr": conn.Laddr,
				"cmd": cmd,
				"users": users,
			}
			fmt.Println(data)
		}
	}

	return nil
}
