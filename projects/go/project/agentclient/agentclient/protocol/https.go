package protocol

import (
	"agentclient/message"
	"context"
	"encoding/binary"
	"log"
	"net"
	"runtime"
	"strconv"
	"time"
)

type httpsClient struct {
	conn net.Conn
}

func (hc *httpsClient) Close() error {
	return hc.conn.Close()
}
func (cli *httpsClient) write(v []byte) (int, error) {
	cli.conn.SetWriteDeadline(time.Now().Add(time.Second))
	return cli.conn.Write(v)
}

func (cli *httpsClient) read(v []byte) (int, error) {
	cli.conn.SetReadDeadline(time.Now().Add(time.Second))
	return cli.conn.Read(v)
}

func (cli *httpsClient) getLocalPort() uint16 {
	laddr := cli.conn.LocalAddr().String()
	if _, port, err := net.SplitHostPort(laddr); err != nil {
		log.Println("getLocalPort error:", err.Error())
		return 0
	} else {
		if v, err := strconv.Atoi(port); err != nil {
			log.Println("getLocalPort atoi error:", err.Error())
			return 0
		} else {
			return uint16(v)
		}

	}
}

type HTTPSProtocol struct {
	source uint16
	port   uint16
	client *httpsClient
}

func NewHTTPSProtocol(source uint16, conn net.Conn) *HTTPSProtocol {
	cli := &httpsClient{conn: conn}
	port := cli.getLocalPort()
	return &HTTPSProtocol{source: source, port: port, client: cli}
}

func (p *HTTPSProtocol) Name() string {
	return "HTTPS"
}

func (p *HTTPSProtocol) Close() error {
	return p.client.Close()
}

func (p *HTTPSProtocol) Service() uint16 {
	return message.HTTPSService
}

func (h *HTTPSProtocol) Write(v []byte) (int, error) {
	retry := 0

	// log.Println("===============================================================")
	// log.Println(string(v))
	// log.Println("===============================================================")
	for {
		if retry >= 10 {
			return 0, ErrNetworkTimeout
		}

		if n, err := h.client.write(v); err != nil {
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

func (hp *HTTPSProtocol) Handle(ctx context.Context, source uint16, fb FeedbackHandler) error {
	defer func() {
		log.Println("HTTPS Handle Over!")
		if p := recover(); p != nil {
			panic(p)
		}

		hp.Close()
	}()

	var buf [defaultBuffSize]byte
	for {
		select {
		case <-ctx.Done():
			return context.Canceled
		default:
			if n, err := hp.client.read(buf[message.MinMessageSize:]); err != nil {
				if e, ok := err.(net.Error); ok && e.Timeout() {
					continue
				}

				log.Println("https protocol handle error: ", err.Error(), " nread:", n)
				return err
			} else {
				length := message.MinMessageSize + n
				binary.BigEndian.PutUint16(buf[0:2], uint16(n+message.MinMessageSize)) ///length
				binary.BigEndian.PutUint16(buf[2:4], message.DataTypeMessage)          ///mType
				binary.BigEndian.PutUint16(buf[4:6], 0)                                ///op
				binary.BigEndian.PutUint16(buf[6:8], hp.source)                        ///srcPort
				binary.BigEndian.PutUint16(buf[8:10], hp.port)                         ///dstPort
				binary.BigEndian.PutUint16(buf[10:12], message.HTTPSService)           ///service

				retry := 0
				nwrite := 0
				for nwrite < length {
					if retry >= 10 {
						log.Println("feedback timeout!")
						return ErrNetworkTimeout
					}

					if nw, err := fb(buf[nwrite:length]); err != nil {
						if e, ok := err.(net.Error); ok && e.Timeout() {
							retry++
							// time.Sleep(1 * time.Millisecond)
							runtime.Gosched()
							continue
						}

						log.Println(err.Error())
						return err
					} else {
						retry = 0
						nwrite += nw
					}
				}
			}
		}
	}

}
