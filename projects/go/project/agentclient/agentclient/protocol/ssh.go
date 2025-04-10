package protocol

import (
	"agentclient/message"
	"context"
	"encoding/binary"
	"log"
	"net"
	"runtime"
	"time"
)

type SSHProtocol struct {
	client *sshClient
}

type sshClient struct {
	conn net.Conn
}

func (cli *sshClient) Close() error {
	return cli.conn.Close()
}

func (cli *sshClient) write(v []byte) (int, error) {
	cli.conn.SetWriteDeadline(time.Now().Add(time.Second))
	return cli.conn.Write(v)
}

func (cli *sshClient) read(v []byte) (int, error) {
	cli.conn.SetReadDeadline(time.Now().Add(time.Second))
	return cli.conn.Read(v)
}

func NewSSHProtocol(conn net.Conn) *SSHProtocol {
	cli := &sshClient{conn: conn}
	return &SSHProtocol{client: cli}
}

func (ssh *SSHProtocol) Name() string {
	return "SSH"
}

func (ssh *SSHProtocol) Close() error {
	return ssh.client.Close()
}

func (ss *SSHProtocol) Service() uint16 {
	return message.SSHService
}

func (ssh *SSHProtocol) Write(v []byte) (int, error) {
	retry := 0

	for {
		if retry >= 10 {
			return 0, ErrNetworkTimeout
		}

		if n, err := ssh.client.write(v); err != nil {
			if e, ok := err.(net.Error); ok && e.Timeout() {
				retry++
				time.Sleep(1 * time.Millisecond)
				continue
			}

			return 0, err
		} else {
			return n, nil
		}
	}
}

func (ssh *SSHProtocol) Handle(ctx context.Context, source uint16, fb FeedbackHandler) error {
	defer func() {
		log.Println("SSH Handle Over!")
		if p := recover(); p != nil {
			panic(p)
		}

		ssh.Close()
	}()

	var buf [defaultBuffSize]byte
	for {
		select {
		case <-ctx.Done():
			return context.Canceled
		default:
			if n, err := ssh.client.read(buf[message.MinMessageSize:]); err != nil {
				if e, ok := err.(net.Error); ok && e.Timeout() {
					continue
				}

				log.Println("ssh protocol handle read err:", err.Error())
				return err
			} else {
				length := n + message.MinMessageSize
				binary.BigEndian.PutUint16(buf[0:2], uint16(n+message.MinMessageSize)) ///length
				binary.BigEndian.PutUint16(buf[2:4], message.DataTypeMessage)          ///mType
				binary.BigEndian.PutUint16(buf[4:6], 0)                                ///op
				binary.BigEndian.PutUint16(buf[6:8], source)                           ///srcPort
				binary.BigEndian.PutUint16(buf[8:10], source)                          ///dstPort
				binary.BigEndian.PutUint16(buf[10:12], message.SSHService)             ///service

				retry := 0
				for {
					if retry >= 10 {
						log.Println("feedback timeout!")
						return ErrNetworkTimeout
					}

					if _, err = fb(buf[:length]); err != nil {
						if e, ok := err.(net.Error); ok && e.Timeout() {
							retry++
							time.Sleep(1 * time.Millisecond)
							runtime.Gosched()
							continue
						}

						log.Println(err.Error())
						return err
					} else {
						retry = 0
						break
					}
				}
			}
		}
	}

}
