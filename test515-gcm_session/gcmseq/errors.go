package gcmseq

import (
	"errors"
)

// ErrInvalidIV is returned by NewSealer and NewOpener if the size of an IV is
// invalid.
var ErrInvalidIV = errors.New("bad IV size")

// ErrInvalidMessage is returned by Open if the format of a message is invalid.
var ErrInvalidMessage = errors.New("invalid message format")

// ErrReplayAttack is returned by Open if the counter value of a message is
// not greater than that of a previously decrypted one.
var ErrReplayAttack = errors.New("message is replayed")
