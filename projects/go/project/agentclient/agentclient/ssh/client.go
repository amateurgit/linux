package ssh

import (
	"sync"
	"time"

	"golang.org/x/crypto/ssh"
)

type SSHClient struct {
	l       sync.Mutex
	conf    *ssh.ClientConfig
	client  *ssh.Client
	session *ssh.Session
}

func NewSSHClient() *SSHClient {
	return &SSHClient{}
}

func Connect() {
	config := &ssh.ClientConfig{
		User:            "yuanwei",
		Auth:            []ssh.AuthMethod{ssh.Password("123456")},
		Timeout:         30 * time.Second,
		HostKeyCallback: ssh.InsecureIgnoreHostKey(),
	}

	_ = config
}
