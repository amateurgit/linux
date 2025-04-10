package protocol

import (
	"context"
	"errors"
	"net"
	"time"
)

const (
	defaultBuffSize        = 65535
	DefaultKeepAlivePeriod = 20 * time.Second
)

var ErrNetworkTimeout = errors.New("network timeout")

type FeedbackHandler func(v []byte) (int, error)

type Protocol interface {
	Name() string
	Close() error
	Service() uint16
	Write(v []byte) (int, error)
	Handle(ctx context.Context, source uint16, fb FeedbackHandler) error
}

func SetConnKeepAlive(conn net.Conn) {
	if c, ok := conn.(*net.TCPConn); ok {
		c.SetKeepAlive(true)
		c.SetKeepAlivePeriod(DefaultKeepAlivePeriod)
	}
}
