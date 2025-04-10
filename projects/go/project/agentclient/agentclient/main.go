package main

import (
	"agentclient/message"
	"agentclient/protocol"
	"context"
	"errors"
	"flag"
	"fmt"
	"log"
	"net"
	"runtime"
	"sync"
	"time"
)

const (
	AgentInit uint16 = iota
	AgentRegister
	AgentReady
)

type agent struct {
	state    uint16
	sn       string
	addr     string
	port     uint16
	conn     net.Conn
	wlock    sync.Mutex
	servLock sync.RWMutex
	services map[uint16]protocol.Protocol
	lastTime time.Time
}

const (
	defaultBuffSize = 65535
)

var ErrUnknowServ = errors.New("unknow server")
var ErrUnknownProto = errors.New("unknown protocol")
var ErrEmptyMessage = errors.New("empty Message")
var ErrInvalidMessage = errors.New("invalid message")
var ErrInvalidRequest = errors.New("invalid Request")

func newAgent(addr, sn string, port uint16, conn net.Conn) *agent {
	serv := make(map[uint16]protocol.Protocol)

	return &agent{state: AgentInit, addr: addr, sn: sn, port: port, conn: conn, services: serv}
}

func (agent *agent) Close() error {
	return agent.conn.Close()
}

func (agent *agent) delService(source uint16) {
	agent.servLock.Lock()
	defer agent.servLock.Unlock()

	delete(agent.services, source)
}

func (agent *agent) isTimeout() bool {
	return time.Now().After(agent.lastTime)
}

func (agent *agent) active() {
	agent.lastTime = time.Now().Add(protocol.DefaultKeepAlivePeriod)
}

func (agent *agent) read(v []byte) (int, error) {
	agent.conn.SetReadDeadline(time.Now().Add(time.Second))
	if n, err := agent.conn.Read(v[:2]); err != nil {
		return 0, err
	} else {
		if n != 2 {
			log.Println("n != 2 >>>>> n:", n)
			return 0, ErrInvalidMessage
		}

		length := message.GetUint16(0, v[:2])
		if length < message.MinMessageSize || length > defaultBuffSize {
			return 0, ErrInvalidMessage
		}

		offset := 2

		for uint16(offset) < length {
			n, err = agent.conn.Read(v[offset:length])
			if err != nil {
				if e, ok := err.(net.Error); ok && e.Timeout() {
					runtime.Gosched()
					continue
				}

				return 0, err
			} else {
				offset += n
			}

		}

		if offset != int(length) {
			return 0, ErrInvalidMessage
		}

		agent.active()

		return int(length), nil
	}
}

func (agent *agent) write(v []byte) (int, error) {
	agent.wlock.Lock()
	defer agent.wlock.Unlock()

	agent.conn.SetWriteDeadline(time.Now().Add(time.Second))
	agent.active()
	return agent.conn.Write(v)
}

func connectServ(addr string, port uint16) (conn net.Conn, err error) {
	servAddr := fmt.Sprintf("%s:%d", addr, port)
	log.Println("Connect server:" + servAddr)
	tcpAddr, err := net.ResolveTCPAddr("tcp", servAddr)
	if err != nil {
		log.Println("connectServ ", err.Error())
		return nil, err
	}

	c, err := net.DialTCP("tcp", nil, tcpAddr)
	if err != nil {
		log.Println("connectSer dial failure ", err.Error())
		return nil, err
	}

	log.Println("connectServ success")
	return c, nil
}

func startHandle(ctx context.Context, srcPort uint16, agent *agent, proto protocol.Protocol) {
	proto.Handle(ctx, srcPort, agent.write)
	agent.delService(srcPort)
}

func (agent *agent) getHandle(source uint16) (protocol.Protocol, bool) {
	agent.servLock.RLock()
	defer agent.servLock.RUnlock()

	proto, ok := agent.services[source]
	return proto, ok
}

func (agent *agent) procMessage(ctx context.Context, v []byte) error {
	length := message.GetUint16(message.MessageLengthOffset, v)
	srcPort := message.GetUint16(message.MessageSrcPortOffset, v)
	dstPort := message.GetUint16(message.MessageDstPortOffset, v)
	service := message.GetUint16(message.MessageServiceOffset, v)
	_ = dstPort
	// log.Println("procMessage length:", length, " data length:", len(v), " service:", service)
	if len(v) < int(length) {
		return ErrInvalidMessage
	}

	if proto, ok := agent.getHandle(srcPort); ok {
		if service != proto.Service() {
			log.Println("Service Error!")
			return ErrInvalidMessage
		}
		if _, err := proto.Write(v[message.MinMessageSize:length]); err != nil {
			return err
		}
	} else {
		var port uint16
		if service == message.SSHService {
			port = 22
		} else if service == message.HTTPSService {
			port = 443
		} else if service == message.WebAPIService {
			port = 30443
		} else if service == message.HTTPService {
			port = 80
		} else {
			return ErrUnknowServ
		}

		if conn, err := connectServ("127.0.0.1", port); err != nil {
			log.Println("Connect local service ", port, " error ", err.Error())
			return err
		} else {
			var proto protocol.Protocol
			if service == message.SSHService {
				proto = protocol.NewSSHProtocol(conn)
			} else if service == message.HTTPSService {
				proto = protocol.NewHTTPSProtocol(srcPort, conn)
			} else if service == message.HTTPService {
				proto = protocol.NewHTTPSProtocol(srcPort, conn)
			} else {
				conn.Close()
				return ErrUnknownProto
			}

			agent.servLock.Lock()
			agent.services[srcPort] = proto
			agent.servLock.Unlock()

			go startHandle(ctx, srcPort, agent, proto)

			if _, err := proto.Write(v[message.MinMessageSize:length]); err != nil {
				log.Println("protocol ", proto.Name(), " write error ", err.Error())
				return err
			}

		}

	}

	return nil
}

func (agent *agent) loop() {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	defer func() {
		log.Println("agent loop exit")
		agent.Close()
	}()

	agent.active()
	timeoutCnt := 0
	var buf [defaultBuffSize]byte
	for {
		switch agent.state {
		case AgentInit:
			laddr := agent.conn.LocalAddr().String()
			ip, _, _ := net.SplitHostPort(laddr)
			msg := fmt.Sprintf("%s#%s", ip, agent.sn)
			m := message.NewMessage(message.CtrlTypeMessage, message.CtrlRegisterRequest, []byte(msg))
			v := message.Marshal(m)
			if _, err := agent.write(v); err != nil {
				log.Println("AgeintInit ", err.Error())
				return
			}

			agent.state = AgentRegister
		case AgentRegister:
			if n, err := agent.read(buf[:]); err != nil {
				if e, ok := err.(net.Error); ok && e.Timeout() {
					if agent.isTimeout() {
						return
					}

					log.Println("AgentRegister ", err.Error())
				} else {
					log.Println("AgentRegister: ", err.Error())
					return
				}
			} else {
				if m, err := message.Unmarshal(buf[:n]); err != nil {
					log.Println(err.Error())
					return
				} else {
					log.Println("Length:", m.Length())
					log.Println("Type:", m.GetType())
					log.Println("OP:", m.GetOP())
					log.Println("Body:", string(m.GetBody()))
					if m.GetType() != message.CtrlTypeMessage {
						log.Println("AgentRegister curr state data invalid")
						return
					}

					if m.GetOP() == message.CtrlRegisterResponse {
						agent.state = AgentReady
					} else {
						return
					}
				}
			}

		case AgentReady:
			if n, err := agent.read(buf[:defaultBuffSize]); err != nil {
				if e, ok := err.(net.Error); ok && e.Timeout() {
					if agent.isTimeout() {
						if timeoutCnt >= 3 {
							log.Println("session timeout.")
							return
						}

						timeoutCnt++

						timeoutMessage := message.NewMessage(message.KeepAliveMessage, message.KeepAliveMessage, []byte("heartbeat"))
						v := message.Marshal(timeoutMessage)
						if _, err = agent.write(v); err != nil {
							if _, ok := err.(net.Error); !ok {
								return
							}
						}
					}

					runtime.Gosched()
					continue
				} else {
					log.Println("agentWaitMessage: ", err.Error())
					return
				}
			} else {
				if m, err := message.Unmarshal(buf[:n]); err != nil {
					log.Println(err.Error())
					return
				} else {
					pType := m.GetType()
					if message.KeepAliveMessage == pType {
						timeoutCnt = 0
					} else {
						if err = agent.procMessage(ctx, buf[:n]); err != nil {
							if err != ErrInvalidMessage && err != ErrUnknowServ {
								log.Println("======== Error ========= ", err.Error())
							}
						}
					}
				}
			}

		default:
			return

		}
	}
}

func startService(addr, sn string, port uint16) {
	for {
		conn, err := connectServ(addr, port)
		if err != nil {
			log.Println("startService connectServ failure ", err.Error())
			time.Sleep(3 * time.Second)
			continue
		}

		protocol.SetConnKeepAlive(conn)

		agent := newAgent(addr, sn, port, conn)
		agent.loop()
		time.Sleep(3 * time.Second)
	}
}

var serv = flag.String("server", "127.0.0.1", "服务器地址")
var port = flag.Int("port", 50505, "注册服务器监听端口")
var sn = flag.String("sn", "APW1KMB00001TEST", "设备SN")

func main() {
	flag.Parse()

	startService(*serv, *sn, uint16(*port))

}
