package wsock

import (
	"errors"
	"strings"
)

var (
	// ErrTimeout represents a session timeout error.
	ErrTimeout = errors.New("websocket session timeout")
)

// isClosedError checks if an error is about use of closed network connection.
func isClosedError(err error) bool {
	// See: https://github.com/golang/go/issues/4373
	return err != nil && strings.Contains(err.Error(), "use of closed network connection")
}
