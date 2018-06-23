package session

import (
	"errors"
	"strings"
)

var (
	// ErrClosed represents an error of using closed websocket connection.
	ErrClosed = errors.New("use of closed websocket connection")
)

// reinterpretError interprets a raw error from websocket package and returns
// a customized error.
func reinterpretError(err error) error {
	if err == nil {
		return nil
	}

	// See: https://github.com/golang/go/issues/4373
	if strings.Contains(err.Error(), "use of closed network connection") {
		return ErrClosed
	}

	return err
}
