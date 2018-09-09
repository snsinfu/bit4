package main

import (
	"bufio"
	"errors"
	"fmt"
	"net"
	"os"
	"os/user"
	"strconv"

	"github.com/snsinfu/bit4/test589-go_qstat/dis"
)

const (
	serverAddress   = "192.168.108.254:15001"
	protocolType    = 2  // PBS_BATCH_PROT_TYPE
	protocolVersion = 2  // PBS_BATCH_PROT_VER
	reqStatusJob    = 19 // PBS_BATCH_StatusJob
	choiceStatus    = 6  // BATCH_REPLY_CHOICE_Status
	objTypeJob      = 2  // MGR_OBJ_JOB

	authAddress        = "/tmp/trqauthd-unix"
	authTypeIFF        = 1 // AUTH_TYPE_IFF
	trqAuthConnection  = 1 // TRQ_AUTH_CONNECTION
	trqGetActiveServer = 2 // TRQ_GET_ACTIVE_SERVER
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	myself, err := user.Current()
	if err != nil {
		return err
	}
	username := myself.Username

	// Request job status
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		return err
	}
	defer conn.Close()

	port := conn.LocalAddr().(*net.TCPAddr).Port
	if err := authenticate(username, port); err != nil {
		return err
	}

	w := bufio.NewWriter(conn)
	r := bufio.NewReader(conn)

	w.WriteString(dis.EncodeInt(protocolType))
	w.WriteString(dis.EncodeInt(protocolVersion))
	w.WriteString(dis.EncodeInt(reqStatusJob))
	w.WriteString(dis.EncodeString(username))

	w.WriteString(dis.EncodeString("")) // Job name (empty means all)
	w.WriteString(dis.EncodeInt(0))     // Number of attributes
	w.WriteString(dis.EncodeInt(0))     // Extension flag

	if err := w.Flush(); err != nil {
		return err
	}

	// Response
	resProtocolType, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	if resProtocolType != protocolType {
		return fmt.Errorf("protocol type mismatch: %d", resProtocolType)
	}

	resProtocolVersion, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	if resProtocolVersion != protocolVersion {
		return fmt.Errorf("protocol version mismatch: %d", resProtocolVersion)
	}

	resCode, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	resAuxCode, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	if resCode != 0 {
		return fmt.Errorf("error code=%d aux=%d", resCode, resAuxCode)
	}

	resChoice, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	if resChoice != choiceStatus {
		return fmt.Errorf("unexpected choice: %d", resChoice)
	}

	count, err := dis.ReadInt(r)
	if err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		objType, err := dis.ReadInt(r)
		if err != nil {
			return err
		}

		objName, err := dis.ReadString(r)
		if err != nil {
			return err
		}

		fmt.Printf("type=%d name=%q\n", objType, objName)

		attrCount, err := dis.ReadInt(r)
		if err != nil {
			return err
		}

		for j := 0; j < int(attrCount); j++ {
			if _, err := dis.ReadInt(r); err != nil {
				return err
			}

			attrName, err := dis.ReadString(r)
			if err != nil {
				return err
			}

			hasResource, err := dis.ReadInt(r)
			if err != nil {
				return err
			}

			var attrResource string
			if hasResource != 0 {
				attrResource, err = dis.ReadString(r)
				if err != nil {
					return err
				}
			}

			attrValue, err := dis.ReadString(r)
			if err != nil {
				return err
			}

			if _, err := dis.ReadInt(r); err != nil {
				return err
			}

			fmt.Printf(" %q [%q]: %q\n", attrName, attrResource, attrValue)
		}
	}

	return nil
}

func authenticate(username string, port int) error {
	// https://github.com/adaptivecomputing/torque/blob/master/src/lib/Libifl/trq_auth.c

	conn, err := net.Dial("unix", authAddress)
	if err != nil {
		return err
	}
	defer conn.Close()

	serverName, serverPortStr, err := net.SplitHostPort(serverAddress)
	if err != nil {
		return err
	}

	serverPort, err := strconv.Atoi(serverPortStr)
	if err != nil {
		return err
	}

	msg := fmt.Sprintf(
		"%d|%d|%s|%d|%d|%d|%s|%d|%d|",
		trqAuthConnection,
		len(serverName),
		serverName,
		serverPort,
		authTypeIFF,
		len(username),
		username,
		os.Getpid(),
		port,
	)
	if _, err := conn.Write([]byte(msg)); err != nil {
		return err
	}

	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil {
		return err
	}

	if string(buf[:n]) != "0|0||" {
		return errors.New(string(buf[:n]))
	}

	return nil
}
