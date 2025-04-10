package message

import (
	"bytes"
	"encoding/binary"
	"errors"
)

const (
	MinMessageSize  = 12
	DefaultBuffSize = 128
)

const (
	InvalidTypeMessage uint16 = iota
	CtrlTypeMessage
	DataTypeMessage
	KeepAliveMessage
)

const (
	InvalidService uint16 = iota
	SSHService
	HTTPSService
	WebAPIService
	HTTPService
)

const (
	InvalidCtrlMessage uint16 = iota
	CtrlRegisterRequest
	CtrlRegisterResponse
	CtrlOpenSSHRequest
	CtrlOpenSSHResponse
	CtrlOpenHTTPSRequest
	CtrlOpenHTTPSResponse
	CtrlOpenWebAPIRequest
	CtrlOpenWebAPIResponse
)

type Message struct {
	length  uint16
	mType   uint16
	op      uint16
	srcPort uint16
	dstPort uint16
	service uint16
	body    []byte
}

var ErrInvalidPacket = errors.New("invalid packet")

func NewMessage(mType, op uint16, body []byte) *Message {
	length := MinMessageSize + len(body)
	return &Message{
		length: uint16(length),
		mType:  mType,
		op:     op,
		body:   body,
	}
}

func (m *Message) Length() uint16 {
	return m.length
}

func (m *Message) GetOP() uint16 {
	return m.op
}

func (m *Message) SetSession(srcPort, dstPort, service uint16) {
	m.srcPort = srcPort
	m.dstPort = dstPort
	m.service = service
}

func (m *Message) GetSession() (srcPort, dstPort, service uint16) {
	return m.srcPort, m.dstPort, m.service
}

func (m *Message) SetBody(v []byte) {
	m.body = append(m.body, v...)
}

func (m *Message) GetBody() []byte {
	return m.body
}

func (m *Message) GetType() uint16 {
	return m.mType
}

func Marshal(m *Message) []byte {
	var buf bytes.Buffer
	binary.Write(&buf, binary.BigEndian, m.length)
	binary.Write(&buf, binary.BigEndian, m.mType)
	binary.Write(&buf, binary.BigEndian, m.op)
	binary.Write(&buf, binary.BigEndian, m.srcPort)
	binary.Write(&buf, binary.BigEndian, m.dstPort)
	binary.Write(&buf, binary.BigEndian, m.service)
	binary.Write(&buf, binary.BigEndian, m.body)

	return buf.Bytes()
}

func Unmarshal(v []byte) (*Message, error) {
	sz := len(v)
	if sz < MinMessageSize {
		return nil, ErrInvalidPacket
	}

	dp := Message{}
	dp.length = binary.BigEndian.Uint16(v[:2])
	dp.mType = binary.BigEndian.Uint16(v[2:4])
	dp.op = binary.BigEndian.Uint16(v[4:6])
	dp.srcPort = binary.BigEndian.Uint16(v[6:8])
	dp.dstPort = binary.BigEndian.Uint16(v[8:10])
	dp.service = binary.BigEndian.Uint16(v[10:12])
	dp.body = append(dp.body, v[12:]...)

	return &dp, nil
}

func GetUint16(pos int, v []byte) uint16 {
	return binary.BigEndian.Uint16(v[pos : pos+2])
}

const (
	MessageLengthOffset  = 0
	MessageTypeOffset    = MessageLengthOffset + 2
	MessageOPOffset      = MessageTypeOffset + 2
	MessageSrcPortOffset = MessageOPOffset + 2
	MessageDstPortOffset = MessageSrcPortOffset + 2
	MessageServiceOffset = MessageDstPortOffset + 2
	MessageBodyOffset    = MessageServiceOffset + 2
)
